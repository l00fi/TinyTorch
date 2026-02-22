#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Vector.h"
#include "Tensor.h"
#include "Tests.h"
#include "TinyTorch.h"

main() {	
	printf("Starting TinyTorch Unit Tests...\n\n");

	RUN_TEST(vector_emptyCorrect_init);
	RUN_TEST(vector_Correct_init);
	RUN_TEST(vector_Empty_arr);
	RUN_TEST(vector_getPositive_index);
	RUN_TEST(vector_getNegative_index);
	RUN_TEST(vector_appendCorrect_works);
	RUN_TEST(vector_editPositive_index);
	RUN_TEST(vector_editNegative_index);
	RUN_TEST(vector_mergeCorrect_works);
	RUN_TEST(vector_mergeWith_zero_size_vector);
	RUN_TEST(vector_mergeWith_zero_size_vectors);
	RUN_TEST(vector_copyCorrect_works);
	RUN_TEST(vector_sumCorrect_works_int);
	RUN_TEST(vector_sumCorrect_works_float);
	RUN_TEST(vector_sumCorrect_works_double);
	RUN_TEST(tensor_Correct_init);
	RUN_TEST(tensor_getCorrect_works);
	RUN_TEST(tensor_sumCorrect_works_int);
	RUN_TEST(tensor_sumCorrect_works_float);
	RUN_TEST(tensor_sumCorrect_works_double);

	print_test_report();

	return (global_state.failed_count > 0);
}