#include "list.h"

inline TCList *_tc_list_new(malloc_f _malloc) {
  TCList *list = (TCList *)_malloc(sizeof(TCList));

  list->_st.prev = &list->_st;
  list->_st.next = &list->_st;

  return list;
}

inline unsigned char tc_list_empty(TCList *list) {
  if (list != NULL) {
    return list->_st.next == list->_st.prev & list->_st.next == &list->_st;
  }
  return 1;
}

TCListIter _tc_list_insert(TCListIter iter, TCListIter target) {
  target->next = iter;
  target->prev = iter->prev;

  if (iter->prev) {
    iter->prev->next = target;
  }

  iter->prev = target;
  return target;
}

inline TCListIter tc_list_end(TCList *list) {
  return list != NULL ? &list->_st : NULL;
}

inline TCListIter tc_list_next(TCListIter iter) {
  return iter != NULL ? iter->next : NULL;
}

inline TCListIter tc_list_prev(TCListIter iter) {
  return iter != NULL ? iter->prev : NULL;
}

#ifdef TANC_UT_ON

#include "utest.h"

typedef struct {
  int v;
} ObjA;

typedef struct {
  int v;
  TCListPos x;
} ObjB;

TCListOf(int);
TCListOf(ObjA);
TCListOf(ObjB, x);

void list_test(UTState *ut_state) {
  TCListIter iter;
  TCList *list1 = tc_list_new();
  TCList *list2 = tc_list_new();
  TCList *list3 = tc_list_new();

  tc_ut("new list", {
    tc_ut_assert(tc_list_empty(list1));
    tc_ut_assert(tc_list_empty(list2));
    tc_ut_assert(tc_list_empty(list3));
  });

  tc_ut("push node", {
    tc_list_push(list1, int, 1);
    tc_ut_assert(!tc_list_empty(list1));
    tc_ut_assert(*tc_list_front(list1, int) == 1);
    tc_ut_assert(*tc_list_back(list1, int) == 1);

    tc_list_push(list2, ObjA, (ObjA){.v = 1});
    tc_ut_assert(!tc_list_empty(list2));
    tc_ut_assert(tc_list_front(list2, ObjA)->v == 1);
    tc_ut_assert(tc_list_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 1;
    tc_list_push(list3, ObjB, ob);
    tc_ut_assert(!tc_list_empty(list3));
    tc_ut_assert(tc_list_front(list3, ObjB)->v == 1);
    tc_ut_assert(tc_list_back(list3, ObjB)->v == 1);
  });

  tc_ut("unshift node", {
    tc_list_unshift(list1, int, 2);
    tc_ut_assert(*tc_list_front(list1, int) == 2);
    tc_ut_assert(*tc_list_back(list1, int) == 1);

    tc_list_unshift(list2, ObjA, (ObjA){.v = 2});
    tc_ut_assert(tc_list_front(list2, ObjA)->v == 2);
    tc_ut_assert(tc_list_back(list2, ObjA)->v == 1);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 2;
    tc_list_unshift(list3, ObjB, ob);
    tc_ut_assert(tc_list_front(list3, ObjB)->v == 2);
    tc_ut_assert(tc_list_back(list3, ObjB)->v == 1);
  });

  tc_ut("insert node", {
    iter = tc_list_next(tc_list_begin(list1));
    iter = tc_list_insert(iter, int, 3);
    tc_ut_assert(*tc_list_at(iter, int) == 3);

    iter = tc_list_next(tc_list_begin(list2));
    iter = tc_list_insert(iter, ObjA, (ObjA){.v = 3});
    tc_ut_assert(tc_list_at(iter, ObjA)->v == 3);

    ObjB *ob = (ObjB *)malloc(sizeof(ObjB));
    ob->v = 3;
    iter = tc_list_next(tc_list_begin(list3));
    iter = tc_list_insert(iter, ObjB, ob);
    tc_ut_assert(tc_list_at(iter, ObjB)->v == 3);
  });

  tc_ut("iterate list from head to tail", {
    int i = 0;
    int expected[] = {2, 3, 1};
    TCListIter begin = tc_list_begin(list1);
    TCListIter end = tc_list_end(list1);

    tc_list_each(begin, end, cur) {
      tc_ut_assert(*tc_list_at(cur, int) == expected[i++]);
    }

    i = 0;
    begin = tc_list_begin(list2);
    end = tc_list_end(list2);

    tc_list_each(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjA)->v == expected[i++]);
    }

    i = 0;
    begin = tc_list_begin(list3);
    end = tc_list_end(list3);

    tc_list_each(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjB)->v == expected[i++]);
    }
  });

  tc_ut("iterate list from tail to head", {
    int i = 2;
    int expected[] = {2, 3, 1};
    TCListIter begin = tc_list_rbegin(list1);
    TCListIter end = tc_list_rend(list1);

    tc_list_reach(begin, end, cur) {
      tc_ut_assert(*tc_list_at(cur, int) == expected[i--]);
    }

    i = 2;
    begin = tc_list_rbegin(list2);
    end = tc_list_rend(list2);

    tc_list_reach(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjA)->v == expected[i--]);
    }

    i = 2;
    begin = tc_list_rbegin(list3);
    end = tc_list_rend(list3);

    tc_list_reach(begin, end, cur) {
      tc_ut_assert(tc_list_at(cur, ObjB)->v == expected[i--]);
    }
  });

  tc_ut("clear list", {
    tc_list_clear(list1, int);
    TCListIter begin = tc_list_begin(list1);
    TCListIter end = tc_list_end(list1);

    tc_ut_assert(tc_list_empty(list1));
    tc_list_each(begin, end, cur) { tc_ut_assert(0); }

    tc_list_clear(list2, ObjA);
    begin = tc_list_begin(list2);
    end = tc_list_end(list2);

    tc_ut_assert(tc_list_empty(list2));
    tc_list_each(begin, end, cur) { tc_ut_assert(0); }

    tc_list_clear(list3, ObjB);
    begin = tc_list_begin(list3);
    end = tc_list_end(list3);

    tc_ut_assert(tc_list_empty(list3));
    tc_list_each(begin, end, cur) { tc_ut_assert(0); }
  });

  tc_ut("free list", {
    tc_list_clear(list1, int);
    list1 = NULL;
    tc_ut_assert(tc_list_empty(list1));

    tc_list_free(list2, ObjA);
    list2 = NULL;
    tc_ut_assert(tc_list_empty(list2));

    tc_list_free(list3, ObjB);
    list3 = NULL;
    tc_ut_assert(tc_list_empty(list3));
  });
}

int main() {
  tc_ut_run(list_test);
  return 0;
}
#endif
