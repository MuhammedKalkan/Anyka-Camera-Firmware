/**
 * 单元测试抽象接口定义。\n
 * 本接口抽象后使用时可兼容 googletest 以及 cunit 两种单元测试方式。\n
 * 用户可以通过在包含本头文件前加入定义\n
 * #define USE_GTEST\n
 * 或\n
 * #define USE_CUNIT\n
 * 去选择使用其中一种，如果没有则会默认使用 googletest 模式进行单元测试。
 */


#if defined (HAS_GTEST_H_)
/**
 * 通过 cmake 检查是否存在 gtest.h 头文件机制判断是否可以使用 googletest。
 */
# define USE_GTEST

#endif
//#define USE_CUNIT
#if !defined (USE_GTEST) & !defined (USE_CUNIT)
# define USE_GTEST
#endif


#if defined (USE_GTEST)
# include <gtest/gtest.h>

#define __UNIT_ASSERT_TRUE       ASSERT_TRUE
#define __UNIT_ASSERT_FALSE      ASSERT_FALSE
#define __UNIT_ASSERT_EQ         ASSERT_EQ
#define __UNIT_ASSERT_NE         ASSERT_NE
#define __UNIT_ASSERT_STREQ      ASSERT_STREQ
#define __UNIT_ASSERT_STRNE      ASSERT_STRNE

#define __UNIT_EXPECT_TRUE       EXPECT_TRUE
#define __UNIT_EXPECT_FALSE      EXPECT_FALSE
#define __UNIT_EXPECT_EQ         EXPECT_EQ
#define __UNIT_EXPECT_NE         EXPECT_NE
#define __UNIT_EXPECT_STREQ      EXPECT_STREQ
#define __UNIT_EXPECT_STRNE      EXPECT_STRNE
#define __UNIT_EXPECT_STRCASEEQ  EXPECT_STRCASEEQ
#define __UNIT_EXPECT_STRCASENE  EXPECT_STRNE

#elif defined (USE_CUNIT)
# include "/usr/local/include/CUnit/Basic.h"

#define __UNIT_ASSERT_TRUE       CU_ASSERT_TRUE_FATAL
#define __UNIT_ASSERT_FALSE      CU_ASSERT_FALSE_FATAL
#define __UNIT_ASSERT_EQ         CU_ASSERT_EQUAL_FATAL
#define __UNIT_ASSERT_NE         CU_ASSERT_NOT_EQUAL_FATAL
#define __UNIT_ASSERT_STREQ      CU_ASSERT_STRING_EQUAL_FATAL
#define __UNIT_ASSERT_STRNE      CU_ASSERT_STRING_NOT_EQUAL_FATAL

#define __UNIT_EXPECT_TRUE       CU_ASSERT_TRUE
#define __UNIT_EXPECT_FALSE      CU_ASSERT_FALSE
#define __UNIT_EXPECT_EQ         CU_ASSERT_EQUAL
#define __UNIT_EXPECT_NE         CU_ASSERT_NOT_EQUAL
#define __UNIT_EXPECT_STREQ      CU_ASSERT_STRING_EQUAL
#define __UNIT_EXPECT_STRNE      CU_ASSERT_STRING_NOT_EQUAL
#define __UNIT_EXPECT_STRCASEEQ
#define __UNIT_EXPECT_STRCASENE

#endif

#include <stdlib.h>
#include <akae_stdlib.h>
#include <akae_malloc.h>

#if !defined(AKAE_UNIT_H_)
#define AKAE_UNIT_H_


/**
 * 单元测试定义。
 */
typedef AK_void (*__UnitTestCase)(AK_Object __UNIT_MALLCTR());


/**
 * 信息输出。
 */
#define __UNIT_INFO(__fmt...) akae_log_tag_info("UTest", ##__fmt)

/**
 * 定义一个用例。
 */
#define __UNIT_DEFINE_CASE(__Test_Case, __stacklen) \
	static AK_void __case_##__Test_Case(AK_Object __UNIT_MALLCTR());\
	AK_void _case_##__Test_Case(AK_void) {\
		AK_Object __UNIT_MALLCTR() = AK_null;\
		AK_size mem_size = __stacklen;\
		AK_bytptr mem_ptr = AK_null;\
		AK_bytptr mem = AK_null;\
		\
		/* 设置随机种子。 */\
		srand (akae_clock_nano());\
		\
		mem = (AK_bytptr)(calloc(mem_size, 1));\
		__UNIT_ASSERT_TRUE(AK_null != mem);\
		__UNIT_INFO("\r\n");\
		__UNIT_MALLCTR() = akae_malloc_create(AK_true, mem, mem_size);\
		__UNIT_ASSERT_TRUE(AK_null != __UNIT_MALLCTR());\
		__UNIT_INFO(">>>> %s", #__Test_Case);\
		__UNIT_INFO("Case Memory( Size = %u/%u ) Setup.", akae_malloc_capacity(__UNIT_MALLCTR()), mem_size);\
		__UNIT_ASSERT_EQ(0, akae_malloc_usage(__UNIT_MALLCTR()));\
		\
		/** 测试用例实现。 */\
		__case_##__Test_Case(__UNIT_MALLCTR());\
		\
		/** 检查内存泄漏。*/\
		mem_size = akae_malloc_usage(__UNIT_MALLCTR());\
		if (mem_size > 0) {\
			__UNIT_INFO("Case Memory Leak %u?", mem_size);\
			__UNIT_MALLCTR()->dump(__UNIT_MALLCTR());\
		} else {\
			__UNIT_INFO("Case Memory All Clear.");\
		}\
		__UNIT_INFO("<<<< %s", #__Test_Case);\
		__UNIT_ASSERT_EQ(0, akae_malloc_usage(__UNIT_MALLCTR()));\
		do {\
			/** 检查内存分配器参数。*/\
			__UNIT_EXPECT_EQ(AK_OK, akae_malloc_release(__UNIT_MALLCTR(), &mem_ptr, &mem_size));\
			__UNIT_EXPECT_EQ(mem, mem_ptr);\
			__UNIT_EXPECT_EQ(__stacklen, mem_size);\
		} while(0);\
		\
		free(mem);\
		mem = AK_null;\
		\
	}\
	static AK_void __case_##__Test_Case(AK_Object __UNIT_MALLCTR())


/**
 * 用例内的内存分配器。
 */
#define __UNIT_MALLCTR() __Mallocator


#if defined (USE_GTEST)

/**
 * 基于 Google Test 激活一个单元测试。
 */
#define __UNIT_ACTIVE_CASE(__Test_Case) \
    static AK_void _case_##__Test_Case(AK_void);\
	TEST(Unit, __Test_Case) {\
    	_case_##__Test_Case();\
	}

#define __UNIT_BEGIN_ACTIVE_CASE()
#define __UNIT_END_ACTIVE_CASE()

#elif defined (USE_CUNIT)
#define __UNIT_ACTIVE_CASE(__Test_Case) \
	do {\
		AK_void _case_##__Test_Case(AK_void);\
		if (NULL == CU_add_test(Suite, #__Test_Case, _case_##__Test_Case)) {\
			CU_cleanup_registry();\
			return CU_get_error();\
		}\
	} while (0)


#define __UNIT_BEGIN_ACTIVE_CASE() \
		int main() {\
			CU_pSuite Suite = NULL;\
			if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();\
			Suite = CU_add_suite(__FILE__, NULL, NULL);\
			if (NULL == Suite) {\
				CU_cleanup_registry();\
				return CU_get_error();\
			}\


#define __UNIT_END_ACTIVE_CASE()\
			CU_basic_set_mode(CU_BRM_VERBOSE);\
			CU_basic_run_tests();\
			CU_cleanup_registry();\
			return CU_get_error();\
		}

#endif




#endif /* UTEST_H_ */
