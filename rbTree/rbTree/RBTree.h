/******************************************************************************
*  @MyClass.cpp  : �������ʵ��
*
*
******************************************************************************
*  @author	:	mt
*  @date	:	2019/02/14
*  @version	:   v0.1
******************************************************************************
*  ģ���б� :
*
*  Class       :
*  Description :
*
******************************************************************************
*  Change History
* ----------------------------------------------------------------------------
*    Date    :  Ver. : Author : Description
* ----------------------------------------------------------------------------
* 2019/02/14 :       : mt : Create
*            :       :        :
******************************************************************************/
#ifndef __MT_RETREE_H__
#define __MT_RETREE_H__

typedef unsigned __int64 uint64_t;
typedef uint64_t KEY_TYPE;

namespace mt {

enum Color {
	RED = 1,
	BLACK,
};

struct rbtree_node;

/* ����������ṹ */
struct rbtree_basic {
	rbtree_node *   right	= nullptr;		//������
	rbtree_node *   left	= nullptr;		//������
	rbtree_node *   parent	= nullptr;
	Color			color;					//�ڵ���ɫ  ����
};

/*��ֵ��*/
struct rbtree_KeyValue { 
	KEY_TYPE  key;
	void * key_raw	= nullptr;
	void * value	= nullptr;
};

/*������Ľڵ�*/
struct rbtree_node {
	rbtree_basic	*  basic	= nullptr;
	rbtree_KeyValue	*  bus		= nullptr;
};

struct rbtree {
	rbtree_node  * root = nullptr;
	rbtree_node  * nil  = nullptr;
};

class RBTree
{
public:
	RBTree();
	~RBTree();

public:
	bool init();
	bool insert(rbtree_node * node);	
	void del_node(rbtree_node * node);

	rbtree_node* init_node(KEY_TYPE key, void * value);
	rbtree_node* find(KEY_TYPE key);
	rbtree_node* del(rbtree_node* node);

	void traversal();											//����
	void close();

private:
	rbtree * m_rbtree = nullptr;

private:
	void rbtree_insert(rbtree_node *node);						//����
	void rbtree_left_rotate(rbtree_node * node);				//����
	void rbtree_right_rotate(rbtree_node * node);				//����
	void rbtree_insert_fixup(rbtree_node * node);				//��ƽ��
	void rbtree_delete_fixup(rbtree_node *node);
	void rbtree_traversal(rbtree_node *node);

	rbtree_node * rbtree_delete(rbtree_node *node);				//ɾ��
	rbtree_node * rbtree_search(KEY_TYPE key);					//����
	rbtree_node * rbtree_successor(rbtree_node *node);
	rbtree_node * rbtree_mini(rbtree_node *node);
	rbtree_node * rbtree_maxi(rbtree_node *node);

private:
	/*32λƽ̨��64λɢ��*/
	uint64_t MurmurHash2_64_32(const void * key, int len, unsigned int seed);

	/*64λƽ̨��64λɢ��*/
	uint64_t MurmurHash2_64_64(const void * key, int len, unsigned int seed);

};

} // namespace mt



#endif