#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "bitmap.h"
#include "hash.h"
#include "list.h"

#define MAX_LEN 50

//#define MIN(a, b) (((a)<(b))?(a):(b))
//#define MAX(a, b) (((a)>(b))?(a):(b))



/*struct list_item{
	struct list_elem elem;
	int data;
};*/

struct hash_item{
	struct hash_elem elem;
	int value;
};


static bool less(const struct list_elem *a, const struct list_elem *b, void *ptr){
	struct list_item *x = list_entry(a, struct list_item, elem);
	struct list_item *y = list_entry(b, struct list_item, elem);
	if(x->data<y->data) return true;
	else return false;
}

static bool less_hash(const struct hash_elem *a, const struct hash_elem *b, void *ptr){
	struct hash_item *x = hash_entry(a, struct hash_item, elem);
	struct hash_item *y = hash_entry(b, struct hash_item, elem);
	if(x->value<y->value) return true;
	else return false;
}

unsigned hash_function(const struct hash_elem *ele, void *ptr){
	struct hash_item *itm = hash_entry(ele, struct hash_item, elem);
	return hash_int(itm->value);
}

unsigned hash_2_function(const struct hash_elem *ele, void *ptr){
	struct hash_item *itm = hash_entry(ele, struct hash_item, elem);
	return hash_int_2(itm->value);
}

static void hash_square(struct hash_elem *ele, void *ptr){
	struct hash_item *itm = hash_entry(ele, struct hash_item, elem);
	itm->value = (itm->value*itm->value);
}

static void hash_triple(struct hash_elem *ele, void *ptr){
	struct hash_item *itm = hash_entry(ele, struct hash_item, elem);	
	itm->value = (itm->value*itm->value*itm->value);
}

static void hash_des(struct hash_elem *ele, void *ptr){
	struct hash_item *itm;
	itm = hash_entry(ele, struct hash_item, elem);
	free(itm);
}

void checkCmd(char *command){
	
	struct bitmap *bitmap[11];
	struct hash hash[11];
	struct list list[11]; 
	
	char *temp1=NULL, *temp2=NULL, *temp3=NULL, *temp4=NULL, *temp5=NULL, *temp6=NULL;

	char cp[MAX_LEN];
	strcpy(cp, command);
	temp1 = strtok(cp, " ");
	if(temp1!=NULL) temp2 = strtok(NULL, " ");
	if(temp2!=NULL) temp3 = strtok(NULL, " ");
	if(temp3!=NULL) temp4 = strtok(NULL, " ");
	if(temp4!=NULL) temp5 = strtok(NULL, " ");
	if(temp5!=NULL) temp6 = strtok(NULL, " ");

	/*printf("%s\n", temp1);
	if(temp2!=NULL) printf("%s\n", temp2);
	if(temp3!=NULL) printf("%s\n", temp3);
	if(temp4!=NULL) printf("%s\n", temp4);*/

	if(!strncmp(command, "create list", 11)){
		//printf("%d\n", (int)temp3[4]-48);
		list_init(&list[(int)temp3[4]-48]);
	}
	else if(!strncmp(command, "create hashtable", 16)){
		hash_init(&hash[(int)temp3[4]-48], hash_function, less_hash, NULL);
	}
	else if(!strncmp(command, "create bitmap", 13)){
		bitmap[(int)temp3[2]-48]=bitmap_create((size_t)(atoi(temp4)));
	}
	else if(!strncmp(command, "dumpdata list", 13)){
		struct list_elem *ele;
		struct list_item *itm;
		if(list_size(&list[(int)temp2[4]-48])>0){
			for(ele=list_begin(&list[(int)temp2[4]-48]);ele!=list_end(&list[(int)temp2[4]-48]);ele=list_next(ele)){
				itm = list_entry(ele, struct list_item, elem);
				printf("%d ", itm->data);
			}
			printf("\n");
		}
	}
	else if(!strncmp(command, "dumpdata hash", 13)){
		struct hash_iterator hi;
		if(hash_size(&hash[(int)temp2[4]-48])>0){
			hash_first(&hi, &hash[(int)temp2[4]-48]);
			while(hash_next(&hi)){
				struct hash_item *ht = hash_entry(hash_cur(&hi), struct hash_item, elem);
				printf("%d ", ht->value);
			}
			printf("\n");
		}
	}
	else if(!strncmp(command, "dumpdata bm", 11)){
		for(int i=0;i<bitmap_size(bitmap[(int)temp2[2]-48]);i++)
			printf("%d", bitmap_test(bitmap[(int)temp2[2]-48], i));
		printf("\n");
	}
	else if(!strncmp(command, "delete list", 11)){
		struct list_elem *ele;
		for(ele=list_begin(&list[(int)temp2[4]-48]);ele!=list_end(&list[(int)temp2[4]-48]);ele=list_next(ele)){
			list_remove(ele);
		}
	}
	else if(!strncmp(command, "delete hash", 11)){
		hash_destroy(&hash[(int)temp2[4]-48], hash_des);
	}
	else if(!strncmp(command, "delete bm", 9)){
		bitmap_destroy(bitmap[(int)temp2[2]-48]);
	}
	//bitmap
	if(!strncmp(command, "bitmap_all", 10)){
		if(bitmap_all(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)))) printf("true\n");
		else printf("false\n");
	}
	else if(!strncmp(command, "bitmap_any", 10)){
		if(bitmap_any(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)))) printf("true\n");
		else printf("false\n");
	}
	else if(!strncmp(command, "bitmap_contains", 15)){
		bool boolean;
		if(!strcmp(temp5, "true")) boolean=true;
		else boolean=false;

		if(bitmap_contains(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), boolean)) printf("true\n");
		else printf("false\n");
	}
	else if(!strncmp(command, "bitmap_count", 12)){
		bool boolean;
		if(!strcmp(temp5, "true")) boolean=true;
		else boolean=false;

		printf("%zu\n", bitmap_count(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), boolean));

	}
	else if(!strncmp(command, "bitmap_dump", 11)){
		bitmap_dump(bitmap[(int)temp2[2]-48]);
	}
	else if(!strncmp(command, "bitmap_expand", 13)){
		bitmap[(int)temp2[2]-48] = bitmap_expand(bitmap[(int)temp2[2]-48], atoi(temp3));
	}
	else if(!strncmp(command, "bitmap_flip", 11)){
		bitmap_flip(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)));
	}
	else if(!strncmp(command, "bitmap_mark", 11)){
		bitmap_mark(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)));
	}
	else if(!strncmp(command, "bitmap_none", 11)){
		if(bitmap_none(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)))) printf("true\n");
		else printf("false\n");
	}
	else if(!strncmp(command, "bitmap_reset", 12)){
		bitmap_reset(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)));
	}

	else if(!strncmp(command, "bitmap_scan_and_flip", 20)){
		if(!strcmp(temp5, "true")) 
			printf("%zu\n", bitmap_scan_and_flip(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), true));
		else
			printf("%zu\n", bitmap_scan_and_flip(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), false));
	}
	else if(!strncmp(command, "bitmap_scan", 11)){
		if(!strcmp(temp5, "true")) 
			printf("%zu\n", bitmap_scan(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), true));
		else 
			printf("%zu\n", bitmap_scan(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), false));
	}
	else if(!strncmp(command, "bitmap_set_multiple", 19)){
		if(!strcmp(temp5, "true")) bitmap_set_multiple(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), true);
		else bitmap_set_multiple(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3)), (size_t)(atoi(temp4)), false);
	}
	else if(!strncmp(command, "bitmap_set_all", 14)){
		if(!strcmp(temp3, "true"))bitmap_set_all(bitmap[(int)temp2[2]-48], true);
		else bitmap_set_all(bitmap[(int)temp2[2]-48], false);
		
	}
	else if(!strncmp(command, "bitmap_set", 10)){
		if(!strcmp(temp4, "true")) bitmap_set(bitmap[(int)temp2[2]-48], atoi(temp3), true);
		else bitmap_set(bitmap[(int)temp2[2]-48], atoi(temp3), false);
	}
	else if(!strncmp(command, "bitmap_size", 11)){
		printf("%zu\n", bitmap_size(bitmap[(int)temp2[2]-48]));
	}
	else if(!strncmp(command, "bitmap_test", 11)){
		if(bitmap_test(bitmap[(int)temp2[2]-48], (size_t)(atoi(temp3))))
			printf("true\n");
		else printf("false\n");
	}
	//hash
	if(!strncmp(command, "hash_insert", 11)){
		struct hash_elem *ele = (struct hash_elem *)malloc(sizeof(struct hash_elem));
		struct hash_item *has = (struct hash_item *)malloc(sizeof(struct hash_item));
		has->value = atoi(temp3);
		hash_insert(&hash[(int)temp2[4]-48], &(has->elem));
	}
	else if(!strncmp(command, "hash_apply", 10)){
		if(!strncmp(temp3, "square", 6)) hash_apply(&hash[(int)temp2[4]-48], hash_square);
		else if(!strncmp(temp3, "triple", 6)) hash_apply(&hash[(int)temp2[4]-48], hash_triple);
	}
	else if(!strncmp(command, "hash_delete", 11)){
		struct hash_item itm;
		itm.value = atoi(temp3);
		hash_delete(&hash[(int)temp2[4]-48], &(itm.elem));
	}
	else if(!strncmp(command, "hash_empty", 10)){
		if(hash_empty(&hash[(int)temp2[4]-48])) printf("true\n");
		else printf("false\n");
	}
	else if(!strncmp(command, "hash_size", 9)){
		printf("%ld\n", hash_size(&hash[(int)temp2[4]-48]));
	}
	else if(!strncmp(command, "hash_clear", 10)){
		hash_clear(&hash[(int)temp2[4]-48], NULL);
	}
	else if(!strncmp(command, "hash_find", 9)){
		struct hash_item itm;
		itm.value = atoi(temp3);
		struct hash_elem *ele = hash_find(&hash[(int)temp2[4]-48], &(itm.elem));
		if(ele!=NULL){
			struct hash_item *pt = hash_entry(ele, struct hash_item, elem);
			printf("%d\n", pt->value);
		}
	}
	else if(!strncmp(command, "hash_replace", 12)){
		struct hash_elem *ele = (struct hash_elem *)malloc(sizeof(struct hash_elem));
		struct hash_item *itm = (struct hash_item *)malloc(sizeof(struct hash_item));
		itm->value =atoi(temp3);
		hash_replace(&hash[(int)temp2[4]-48], &(itm->elem));
	}
	//list
	if(!strncmp(command, "list_push_back", 14)){
		//printf("%d %d\n", (int)(temp2[4]-48), atoi(temp3));
		struct list_elem *ele = (struct list_elem *)malloc(sizeof(struct list_elem));
		struct list_item *itm = (struct list_item *)malloc(sizeof(struct list_item));
		itm->elem = *ele;
		itm->data = atoi(temp3);
		list_push_back(&list[(int)(temp2[4]-48)] , &(itm->elem));
	}
	else if(!strncmp(command, "list_push_front", 15)){
		struct list_elem *ele = (struct list_elem *)malloc(sizeof(struct list_elem));
		struct list_item *itm = (struct list_item *)malloc(sizeof(struct list_item));
		itm->elem = *ele;
		itm->data = atoi(temp3);
		list_push_front(&list[(int)temp2[4]-48], &(itm->elem));
	}
	else if(!strncmp(command, "list_front", 10)){
		if(!list_empty(&list[(int)temp2[4]-48])){
			struct list_elem *ele = list_front(&list[(int)temp2[4]-48]);
			struct list_item *itm = list_entry(ele, struct list_item, elem);
			printf("%d\n", itm->data);
		}
	}
	else if(!strncmp(command, "list_back", 9)){
		if(!list_empty(&list[(int)temp2[4]-48])){
			struct list_elem *ele = list_back(&list[(int)temp2[4]-48]);
			struct list_item *itm = list_entry(ele, struct list_item, elem);
			printf("%d\n", itm->data);
		}
	}
	else if(!strncmp(command, "list_pop_back", 13)){
		if(!list_empty(&list[(int)temp2[4]-48])){
			struct list_elem *ele = list_pop_back(&list[(int)temp2[4]-48]);
		}
	}
	else if(!strncmp(command, "list_pop_front", 14)){
		if(!list_empty(&list[(int)temp2[4]-48])){
			struct list_elem *ele = list_pop_front(&list[(int)temp2[4]-48]);
		}
	}
	else if(!strncmp(command, "list_insert_ordered", 19)){
		struct list_elem *ele = (struct list_elem *)malloc(sizeof(struct list_elem));
		struct list_item *itm = (struct list_item *)malloc(sizeof(struct list_item));
		itm->elem = *ele;
		itm->data = atoi(temp3);
		list_insert_ordered(&list[(int)temp2[4]-48], &(itm->elem), less, NULL);
	}
	else if(!strncmp(command, "list_insert", 11)){
		struct list_elem *ele = (struct list_elem *)malloc(sizeof(struct list_elem));
		struct list_item *itm = (struct list_item *)malloc(sizeof(struct list_item));
		itm->elem = *ele;
		itm->data = atoi(temp4);
		if(atoi(temp3)==0){
			list_insert(list_begin(&list[(int)temp2[4]-48]), &(itm->elem));
		}
		else if(atoi(temp3)>0){
			int i=0;
			for(ele=list_begin(&list[(int)temp2[4]-48]);ele!=list_end(&list[(int)temp2[4]-48]);ele=list_next(ele)){
				i++;
				if(i==atoi(temp3)){
					struct list_item *tp = list_entry(ele, struct list_item, elem);
					list_insert(tp->elem.next, &(itm->elem));
					break;
				}
			}
		}
	}
	else if(!strncmp(command, "list_empty", 10)){
		if(list_empty(&list[(int)temp2[4]-48])) printf("true\n");
		else printf("false\n");
	}
	else if(!strncmp(command, "list_size", 9)){
		if(!list_empty(&list[(int)temp2[4]-48]))
			printf("%ld\n", list_size(&list[(int)temp2[4]-48]));
	}
	else if(!strncmp(command, "list_max", 8)){
		if(!list_empty(&list[(int)temp2[4]-48])){
			struct list_elem *ele = list_max(&list[(int)temp2[4]-48], less, NULL);
			struct list_item *itm = list_entry(ele, struct list_item, elem);
			printf("%d\n", itm->data);
		}
	}
	else if(!strncmp(command, "list_min", 8)){
		if(!list_empty(&list[(int)temp2[4]-48])){
			struct list_elem *ele = list_min(&list[(int)temp2[4]-48], less, NULL);
			struct list_item *itm = list_entry(ele, struct list_item, elem);
			printf("%d\n", itm->data);
		}
	}
	else if(!strncmp(command, "list_remove", 11)){
		struct list_elem *ele; int i=0;
		for(ele=list_begin(&list[(int)temp2[4]-48]);ele!=list_end(&list[(int)temp2[4]-48]);ele=list_next(ele)){
			if(i==atoi(temp3)){
				struct list_item *tp = list_entry(ele, struct list_item, elem);
				list_remove(&tp->elem);
			}
			i++;
		}
	}
	else if(!strncmp(command, "list_reverse", 12)){
		list_reverse(&list[(int)temp2[4]-48]);
	}
	else if(!strncmp(command, "list_shuffle", 12)){
		list_shuffle(&list[(int)temp2[4]-48]);
	}
	else if(!strncmp(command, "list_sort", 9)){
		list_sort(&list[(int)temp2[4]-48], less, NULL);
	}
	else if(!strncmp(command, "list_splice", 11)){
		struct list_elem *ele, *tp1, *tp2, *tp3;
		int i=0;
		for(ele=list_begin(&list[(int)temp2[4]-48]);ele!=list_end(&list[(int)temp2[4]-48]);ele=list_next(ele)){
			if(i==atoi(temp3)){
				tp1=ele;
				break;
			}
			i++;
		}
		i=0;
		for(ele=list_begin(&list[(int)temp4[4]-48]);ele!=list_end(&list[(int)temp4[4]-48]);ele=list_next(ele)){
			if(i==atoi(temp5)) tp2=ele;
			else if(i==atoi(temp6)) tp3 = ele;
			i++;
		}
		list_splice(tp1, tp2, tp3);
	}
	else if(!strncmp(command, "list_swap", 9)){
		struct list_elem *ele, *tp1, *tp2;
		int i=0;
		for(ele=list_begin(&list[(int)temp2[4]-48]);ele!=list_end(&list[(int)temp2[4]-48]);ele=list_next(ele)){
			if(i==atoi(temp3)) tp1 = ele;
			if(i==atoi(temp4)) tp2 = ele;
			i++;
		}
		list_swap(tp1, tp2);
	}
	else if(!strncmp(command, "list_unique", 11)){
		if(temp3!=NULL)
			list_unique(&list[(int)temp2[4]-48], &list[(int)temp3[4]-48], less, NULL);
		else
			list_unique(&list[(int)temp2[4]-48], NULL, less, NULL);
	}	
}

int main(){
	char arr[MAX_LEN];
	
	while(1){
		fgets(arr, sizeof(arr), stdin);
		arr[strlen(arr)-1]='\0';
		for(int i=strlen(arr)-1;i>0;i--){
			if(arr[i]==' ') arr[i]='\0';
			else break;
		}
		if(strlen(arr)==0) continue;
		if(!strcmp(arr, "quit")) break;
		checkCmd(arr);
	}

	return 0;
}
