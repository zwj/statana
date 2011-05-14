#ifndef __TEST_SUPPORT__
#define __TEST_SUPPORT__
// support to unit test;
template <typename VALUE_TYPE>
class class_mock_test
{
        public:
            static void expectation(const VALUE_TYPE& value)
            {
                _s_original_value=VALUE_TYPE();
                _s_expect_value=value;
            }
            static bool verify()
            {
                return _s_original_value==_s_expect_value;
            }
        protected:
            static VALUE_TYPE _s_original_value;
            static VALUE_TYPE _s_expect_value;
            static boost::mutex  _s_access_mutex;
};
template <typename VALUE_TYPE>
VALUE_TYPE class_mock_test<VALUE_TYPE>::_s_expect_value;
template <typename VALUE_TYPE>
VALUE_TYPE class_mock_test<VALUE_TYPE>::_s_original_value; 
template <typename VALUE_TYPE>
boost::mutex class_mock_test<VALUE_TYPE>::_s_access_mutex; 

template <typename VALUE_TYPE>
class obj_mock_test
{
        public:
            void expectation(const VALUE_TYPE& value)
            {
                _expect_value=value;
            }
            bool verify()
            {
                return _original_value==_expect_value;
            }
        protected:
            static VALUE_TYPE _original_value;
            static VALUE_TYPE _expect_value;
};

#endif
