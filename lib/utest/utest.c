#include "utest.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

// run test handlers
void _tc_ut_run(tc_ut_handler *ut_handler, size_t len) {
  UTState *state = (UTState *)malloc(sizeof(UTState));

  state->file = NULL;
  state->name = NULL;
  state->assert_rt = NULL;
  state->passed = 1;

  for (int i = 0; i < len; i++) {
    (**ut_handler)(state);
    ut_handler++;
  }

  // output last file test result
  _tc_ut_out(state);
}

// init file test result
void _tc_ut_fs(UTState *state, char *file) {
  if (state->file == NULL || strcmp(state->file, file) != 0) {
    if (state->file != NULL) {
      // output previous file test result
      _tc_ut_out(state);
    }

    state->file = file;
    state->passed = 1;

    if (state->assert_rt) {
      free(state->assert_rt);
    }
    state->assert_rt = tc_list_new();
  }
}

// run test case
inline void _tc_ut(UTState *state, char *name) { state->name = name; }

// store assert result
inline void _tc_ut_assert(UTState *state, int lno, unsigned char passed) {
  state->passed &= passed;

  TCAssertRt rt = {
      .lno = lno,
      .name = state->name,
      .passed = passed,
  };
  tc_list_push(state->assert_rt, TCAssertRt, rt);

  if (state->passed == 0) {
    _tc_ut_out(state);
  }
}

// safely abort with error code
void _tc_ut_abort(UTState *state, int err) {
  tc_list_free(state->assert_rt, TCAssertRt);

  free(state);

  exit(err);
}

// output test result
void _tc_ut_out(UTState *state) {
  if (state->file == NULL || state->assert_rt == NULL ||
      tc_list_empty(state->assert_rt) == 1) {
    fprintf(stderr, "No test cases or assertions found!\n");
    _tc_ut_abort(state, EIO);
  }

  fprintf(stdout, "%s %s\n\n", state->passed ? "[PASS]" : "[FAIL]",
          state->file);

  tc_list_each(tc_list_begin(state->assert_rt), tc_list_end(state->assert_rt),
               iter) {
    TCAssertRt *assert_rt = tc_list_at(iter, TCAssertRt);
    TCAssertRt *next_assert_rt = tc_list_at(tc_list_next(iter), TCAssertRt);

    if (!next_assert_rt || assert_rt->name != next_assert_rt->name) {
      fprintf(assert_rt->passed ? stdout : stderr, "       %s %s\n",
              assert_rt->passed ? "✔" : "✘", assert_rt->name);

      if (assert_rt->passed == 0) {
        fprintf(stderr, "\n         Error: line %d\n", assert_rt->lno);
        _tc_ut_abort(state, EIO);
      }
    }
  }
}
