////////////////////////////////////////////////////////////////////////////////
//

#include <stddef.h>
#include "fortedef.h"

size_t	sys_sbrk_size = SYS_HEAP_SIZE;
char	sys_heap[SYS_HEAP_SIZE];
char*	sys_sbrk_break = (char*)sys_heap;
//void*	sys_allocb = (void*)0;				// declare startup routine

struct	free_list *sys_allocb = 0;

struct	free_list {
	struct free_list*	free_addr;
	size_t				free_size;
};

#define	FLSIZE	(sizeof(struct free_list))

////////////////////////////////////////
//
//
void* sys_sbrk(size_t size)
{
	void *addr;

	// size==0 : return free size
	//
	if(!size)
		return (void *)sys_sbrk_size;

	// enough memory ?
	//
	if(sys_sbrk_size < size)
		return (void *)-1L;

	// allocate
	addr = (void *)sys_sbrk_break;
	sys_sbrk_break += size;			// inclease break address
	sys_sbrk_size  -= size;			// decrease free size

	return addr;
}

////////////////////////////////////////
//
//
void* sys_malloc(size_t allocsz)
{
	struct	free_list	*pp, *cp, *np;
	size_t	size;

	// 4バイトアライメント
	//
	size = (size_t)((allocsz + 3) / 4) * 4;

	// sys_allocb(freeしたchunk)が有効で充分な容量があるか
	//
	for (pp = sys_allocb ; pp ; cp = pp, pp = pp->free_addr) {
		if (pp->free_size >= size)
			break;
	}

	// sys_allocb(freeしたchunk)が有効で充分な容量があるか
	//
	if (pp) {
		if ((pp->free_size - size) >= (FLSIZE + 4)) {
			np = (struct free_list *) (((char *) pp) + FLSIZE + size);
			np->free_addr = pp->free_addr;
			np->free_size = pp->free_size - FLSIZE - size;
			pp->free_addr = np;
			pp->free_size = size;
		}
		if (pp == sys_allocb)
			sys_allocb = pp->free_addr;
		else
			cp->free_addr = pp->free_addr;

	// sys_allocb(freeしたchunk)が無効か充分な容量がない→新規
	//
	} else {
		if ((pp = (struct free_list *) sys_sbrk(size + FLSIZE)) == (struct free_list *) -1)
			return ((void *)NULL);
		pp->free_addr = NULL;
		pp->free_size = size;
	}
	return	((void *) ++pp);
}

////////////////////////////////////////
//
//
void sys_free(void *ptr)
{
	struct free_list* cp;
	struct free_list* pp;
	struct free_list* fp;

	if(!ptr)
		return;

	// ポインタからchunk先頭へ
	//
	fp = (struct free_list*)ptr;
	--fp;

	// sys_allocb(freeしたchunk)が無効
	//
	if (!sys_allocb) {
		fp->free_addr = NULL;
		sys_allocb = fp;
		return ;
	}

	// chunkチェインが有効なら、辿る
	//
	for (pp = cp = sys_allocb ; cp ; pp = cp, cp = cp->free_addr)
		if (fp <= cp)
			break;

	if (fp == cp)
		return;

	if (cp == sys_allocb) {
		if ((struct free_list*) (((char *)fp) + FLSIZE + fp->free_size) == sys_allocb) {
			fp->free_addr = sys_allocb->free_addr;
			fp->free_size += sys_allocb->free_size + FLSIZE;
		} else
			fp->free_addr = sys_allocb;

		sys_allocb = fp;
		return;
	}

	if (cp && ((struct free_list*) (((char *)fp) + FLSIZE + fp->free_size)) == cp) {
		fp->free_addr = cp->free_addr;
		fp->free_size += cp->free_size + FLSIZE;

	} else
		fp->free_addr = cp;

	if ((struct free_list*) (((char *)pp) + FLSIZE + pp->free_size) == fp) {
		pp->free_addr = fp->free_addr;
		pp->free_size += fp->free_size + FLSIZE;

	} else
		pp->free_addr = fp;
}
