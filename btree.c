#include "btree.h"

static btree_node * btree_new_node() {
	btree_node *new_node = malloc(sizeof(btree_node));
	new_node->keys = 0;
    new_node->childs = 0;
	new_node->idx = 0;
	new_node->parent = NULL;
	return new_node;
}

btree * btree_new(int (*compare)(const any *, const any *)) {
	btree * new_tree = malloc(sizeof(btree));
	new_tree->nodes = 1;
	new_tree->keys = 0;
	new_tree->compare = compare;
	new_tree->root = btree_new_node();
	return new_tree;
}

void btree_free(btree * tree) {
    btree_node * cur = tree->root, * p;
    while (cur) {
        if (cur->childs > 0) {
            cur = cur->child[cur->childs - 1];
        }
        else {
            p = cur->parent;
            if (p) p->childs--;
            free(cur);
            cur = p;
        }
    }
    free(tree);
}

static void btree_node_insert_key(btree_node * node, int idx, any * new_key) {
	if (idx < node->keys)
		memmove(&node->key[idx+1], &node->key[idx], sizeof(any)*(node->keys-idx));
	node->key[idx] = *new_key;
	node->keys++;
}

static void btree_node_insert_child(btree_node * node, int idx, btree_node * new_child) {
	if (idx < node->childs) {
		memmove(&node->child[idx+1], &node->child[idx], sizeof(btree_node *)*(node->childs-idx));
		for (int i = idx + 1; i < node->childs + 1; i++)
			node->child[i]->idx++;
	}
	node->child[idx] = new_child;
	node->childs++;
	new_child->idx = idx;
	new_child->parent = node;
}

static void btree_split(btree * tree, btree_node * node) {
	btree_node * new_node = btree_new_node();
	btree_node * parent = node->parent;
	int new_keys = (node->keys - 1) / 2;
	int new_childs = node->childs / 2;

	if (parent == NULL) {
		parent = btree_new_node();
		node->parent = parent;
		tree->nodes++;
		tree->root = parent;
		btree_node_insert_child(parent, 0, node);
	}
	btree_node_insert_key(parent, node->idx, &node->key[node->keys/2]);
	btree_node_insert_child(parent, node->idx+1, new_node);

	for(int i = 0; i < new_keys; i++)
		btree_node_insert_key(new_node, i, &node->key[node->keys/2+i+1]);
	for(int i = 0; i < new_childs; i++)
		btree_node_insert_child(new_node, i, node->child[node->childs/2+i]);

	node->keys = new_keys;
	node->childs = new_childs;
	tree->nodes++;
}

static inline bool btree_node_full(const btree_node * x) {
	return x->keys == BTREE_T * 2 - 1;
}

static int btree_node_find_pos(const btree * tree, const btree_node * node, const any * key) {
    int left = 0, right = node->keys - 1;
    int mid = 0, cmp = 0;
    while (left <= right) {
        mid = (left + right) / 2;
        cmp = tree->compare(key, &node->key[mid]);
        if (cmp == 0) {
            return mid;
        }
        else if (cmp < 0) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return cmp <= 0 ? mid : mid + 1;
}

void btree_insert(btree * tree, any key, any value) {
	btree_node * cur_node = tree->root;
	int idx;
	while (true) {
        // split full node
		if (btree_node_full(cur_node)) {
			btree_split(tree, cur_node);
			cur_node = cur_node->parent;
		}
        // find the position to insert key
		//for (idx = 0; idx < cur_node->keys; idx++) {
		//	if (tree->compare(&key, &cur_node->key[idx]) < 0)
        //        break;
        //}
        idx = btree_node_find_pos(tree, cur_node, &key);
        // insert if it is leaf node
		if (cur_node->childs == 0) {
			btree_node_insert_key(cur_node, idx, &key);
			break;
		}
		else {
            cur_node = cur_node->child[idx];
        }
	}
	tree->keys++;
}

static btree_node * btree_max_node(btree_node * root) {
	while (root->childs > 0) {
		root = root->child[root->childs-1];
    }
	return root;
}

static btree_node * btree_min_node(btree_node * root) {
	while (root->childs > 0) {
        root = root->child[0];
    }
	return root;
}

static btree_node * btree_find(const btree * tree, any * key, int * idx) {
	btree_node * cur_node = tree->root;
	int i;
	while (true) {
        /*
		for (i = 0; i < cur_node->keys; i++) {
            int cmp = tree->compare(key, &cur_node->key[i]);
			if (cmp == 0) {
				*idx = i;
				return cur_node;
			}
			else if (cmp < 0) break;
		}
        */
        i = btree_node_find_pos(tree, cur_node, key);
        if (tree->compare(key, &cur_node->key[i]) == 0) {
            *idx = i;
            return cur_node;
        }
        else if (cur_node->childs > 0) {
            cur_node = cur_node->child[i];
        }
		else {
            return NULL;
        }
	}
}

bool btree_exist(const btree * tree, any key) {
    int idx;
    return btree_find(tree, &key, &idx) ? true : false;
}

any btree_get(const btree * tree, any key) {
    int idx;
    btree_node * node = btree_find(tree, &key, &idx);
    if (node) {
        return node->key[idx];
    }
    else {
        return ANY_NONE;
    }
}

static void btree_node_delete_key(btree_node * x, int idx) {
	if (idx < x->keys - 1) {
		memmove(&x->key[idx], &x->key[idx+1], sizeof(any)*(x->keys-1-idx));
    }
	x->keys--;
}

static void btree_node_delete_child(btree_node * x, int idx) {
	if (idx < x->childs - 1) {
		memmove(&x->child[idx], &x->child[idx+1], sizeof(btree_node *)*(x->childs-1-idx));
		for(int i = idx; i < x->childs - 1; i++)
			x->child[i]->idx--;
	}
	x->childs--;
}

static void btree_remove_node(btree * tree, btree_node * x) {
	free(x);
	tree->nodes--;
}

static void btree_join(btree * tree, btree_node * x, btree_node * y) {
	btree_node * p = x->parent;
	btree_node_insert_key(x, x->keys, &p->key[x->idx]);
	for(int i = 0; i < y->keys; i++)
		btree_node_insert_key(x, x->keys, &y->key[i]);
	for(int i = 0; i < y->childs; i++)
		btree_node_insert_child(x, x->childs, y->child[i]);

	btree_node_delete_key(p, x->idx);
	btree_node_delete_child(p, y->idx);
	btree_remove_node(tree, y);
}

static void btree_adjust(btree * tree, btree_node * x) {
	btree_node * p, * left_bro, * right_bro;

	while (true) {
		p = x->parent;
		if (p == NULL) {
			if (x->keys == 0 && x->childs > 0) {
				tree->root = x->child[0];
				btree_remove_node(tree, x);
			}
			return;
		}

		left_bro = x->idx > 0 ? p->child[x->idx-1] : NULL;
		right_bro = x->idx < p->childs - 1 ? p->child[x->idx+1] : NULL;
		if (right_bro && right_bro->keys > BTREE_T - 1) {
			btree_node_insert_key(x, x->keys, &p->key[x->idx]);
			p->key[x->idx] = right_bro->key[0];
			btree_node_delete_key(right_bro, 0);
			if (x->childs > 0) {
				btree_node_insert_child(x, x->childs, right_bro->child[0]);
				btree_node_delete_child(right_bro, 0);
			}
			return;
		}
		else if (left_bro && left_bro->keys > BTREE_T - 1) {
				btree_node_insert_key(x, 0, &p->key[x->idx-1]);
				p->key[x->idx-1] = left_bro->key[left_bro->keys-1];
				btree_node_delete_key(left_bro, left_bro->keys-1);
				if (x->childs > 0) {
					btree_node_insert_child(x, 0, left_bro->child[left_bro->childs-1]);
					btree_node_delete_child(left_bro, left_bro->childs-1);
				}
				return;
		}
		else if (left_bro) {
				btree_join(tree, left_bro, x);
				if (p->keys < BTREE_T - 1) x = p;
				else return;
		}
		else {
			btree_join(tree, x, right_bro);
			if (p->keys < BTREE_T - 1) x = p;
			else return;
		}
	}
}

void btree_delete(btree * tree, any key) {
	int idx;
	btree_node * del_node = btree_find(tree, &key, &idx);

	if (del_node == NULL) {
		return;
    }
	else if (del_node->childs == 0) {
		btree_node_delete_key(del_node, idx);
	}
	else {
		btree_node *pre = btree_max_node(del_node->child[idx]);
		btree_node *suc = btree_min_node(del_node->child[idx+1]);

		if (pre->keys >= suc->keys) {
			del_node->key[idx] = pre->key[pre->keys-1];
			btree_node_delete_key(pre, pre->keys - 1);
			del_node = pre;
		}
		else {
			del_node->key[idx] = suc->key[0];
			btree_node_delete_key(suc, 0);
			del_node = suc;
		}
	}
    // adjust node when when keys are not enough
	if (del_node->keys < BTREE_T - 1) {
        btree_adjust(tree, del_node);
    }
	tree->keys--;
}
