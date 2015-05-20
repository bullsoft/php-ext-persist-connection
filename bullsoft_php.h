#ifndef BULLSOFT_PHP_H
#define BULLSOFT_PHP_H

#ifndef PHP_FE_END
#define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

#ifndef ZEND_MOD_END
#define ZEND_MOD_END { NULL, NULL, NULL, 0 }
#endif

#if PHP_VERSION_ID >= 50400
#define BULLSOFT_INIT_FUNCS(class_functions) static const zend_function_entry class_functions[] =
#else
#define BULLSOFT_INIT_FUNCS(class_functions) static const function_entry class_functions[] =
#endif

#define BULLSOFT_INIT_CLASS(extname, name)                   \
    int extname## _ ##name## _init(INIT_FUNC_ARGS)

#define BULLSOFT_INIT(extname, name)                                        \
    if (extname## _ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) { \
        return FAILURE; \
    }

#define BULLSOFT_REGISTER_CLASS(extname, class_name, name, methods, flags)   \
  { \
    zend_class_entry ce; \
    memset(&ce, 0, sizeof(zend_class_entry)); \
    INIT_CLASS_ENTRY(ce, #class_name, methods); \
    extname## _ ##name## _ce = zend_register_internal_class(&ce TSRMLS_CC); \
    extname## _ ##name## _ce->ce_flags |= flags;  \
  }

#define HASH_P(a) (Z_TYPE_P(a) == IS_ARRAY ? Z_ARRVAL_P(a) : Z_OBJPROP_P(a))
#define HASH_PP(a) (Z_TYPE_PP(a) == IS_ARRAY ? Z_ARRVAL_PP(a) : Z_OBJPROP_PP(a))

#define IS_SCALAR_P(a) (Z_TYPE_P(a) == IS_NULL || Z_TYPE_P(a) == IS_LONG || Z_TYPE_P(a) == IS_DOUBLE || Z_TYPE_P(a) == IS_BOOL || Z_TYPE_P(a) == IS_STRING)
#define IS_SCALAR_PP(a) IS_SCALAR_P(*a)
#define IS_ARRAY_OR_OBJECT_P(a) (Z_TYPE_P(a) == IS_ARRAY || Z_TYPE_P(a) == IS_OBJECT)

/* TODO: this should be expanded to handle long_as_object being set */
#define Z_NUMVAL_P(variable, value)                                     \
  ((Z_TYPE_P(variable) == IS_LONG && Z_LVAL_P(variable) == value) ||    \
   (Z_TYPE_P(variable) == IS_DOUBLE && Z_DVAL_P(variable) == value))
#define Z_NUMVAL_PP(variable, value)                                    \
  ((Z_TYPE_PP(variable) == IS_LONG && Z_LVAL_PP(variable) == value) ||  \
   (Z_TYPE_PP(variable) == IS_DOUBLE && Z_DVAL_PP(variable) == value))

#define BULLSOFT_EXCEPTION(msg) zend_throw_exception(zend_exception_get_default(TSRMLS_C), #msg, 0 TSRMLS_CC);


#if PHP_VERSION_ID >= 50300
#define BULLSOFT_PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
#define BULLSOFT_POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_C)
#define BULLSOFT_PUSH_EO_PARAM()
#define BULLSOFT_POP_EO_PARAM()
#else
#define BULLSOFT_PUSH_PARAM(arg) zend_ptr_stack_push(&EG(argument_stack), arg)
#define BULLSOFT_POP_PARAM() (void)zend_ptr_stack_pop(&EG(argument_stack))
#define BULLSOFT_PUSH_EO_PARAM() zend_ptr_stack_push(&EG(argument_stack), NULL)
#define BULLSOFT_POP_EO_PARAM() (void)zend_ptr_stack_pop(&EG(argument_stack))
#endif

#define BULLSOFT_CALL_METHOD_BASE(classname, name) zim_##classname##_##name
 
#define BULLSOFT_CALL_METHOD_HELPER(classname, name, retval, thisptr, num, param)      \
  BULLSOFT_PUSH_PARAM(param); BULLSOFT_PUSH_PARAM((void*)num);                         \
  BULLSOFT_PUSH_EO_PARAM();                                                            \
  BULLSOFT_CALL_METHOD_BASE(classname, name)(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
  BULLSOFT_POP_EO_PARAM();                                                             \
  BULLSOFT_POP_PARAM();                                                                \
  BULLSOFT_POP_PARAM();
 
#define BULLSOFT_CALL_METHOD(classname, name, retval, thisptr)                         \
  BULLSOFT_CALL_METHOD_BASE(classname, name)(0, retval, NULL, thisptr, 0 TSRMLS_CC);
 
#define BULLSOFT_CALL_METHOD1(classname, name, retval, thisptr, param1)        \
  BULLSOFT_CALL_METHOD_HELPER(classname, name, retval, thisptr, 1, param1);
 
#define BULLSOFT_CALL_METHOD2(classname, name, retval, thisptr, param1, param2)  \
  BULLSOFT_PUSH_PARAM(param1);                                                   \
  BULLSOFT_CALL_METHOD_HELPER(classname, name, retval, thisptr, 2, param2);      \
  BULLSOFT_POP_PARAM();
 
#define BULLSOFT_CALL_METHOD3(classname, name, retval, thisptr, param1, param2, param3) \
  BULLSOFT_PUSH_PARAM(param1);                                                          \
  BULLSOFT_PUSH_PARAM(param2);                                                          \
  BULLSOFT_CALL_METHOD_HELPER(classname, name, retval, thisptr, 3, param3);             \
  BULLSOFT_POP_PARAM();                                                                 \
  BULLSOFT_POP_PARAM();

#if PHP_VERSION_ID >= 50400
#define bullsoft_init_properties(intern) object_properties_init(&intern->std, class_type)
#else
#define bullsoft_init_properties(intern) {                              \
    zval *tmp;                                                          \
    zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *)); \
}
#endif

#define MK_STD(var)                             \
  zval *var;                                    \
  MAKE_STD_ZVAL(var);                             

#define MK_STD_STRING(var, val)                 \
  MK_STD(var);                                  \
  ZVAL_STRING(var, val, 1);

#define MK_STD_INT(var, val)                    \
  MK_STD(var);                                  \
  ZVAL_LONG(var, val);

#define MK_STD_DOUBLE(var, val)                 \
  MK_STD(var);                                  \
  ZVAL_DOUBLE(var, val);

#define MK_STD_BOOL(var, val)                   \
  MK_STD(var);                                  \
  ZVAL_BOOL(var, val);

#define MK_STD_ARRAY(var)                       \
  MK_STD(var);                                  \
  array_init(var);

#define BULLSOFT_CALL_FUNC_HEAD()               \
  zend_fcall_info fci;                          \
  zend_fcall_info_cache fcc;                    \
  zval function_name;                           \
  ZVAL_STRING(&function_name, func, 1);         \
  zval *ret;                                    \
  zval **argv[param_count];

#define BULLSOFT_CALL_FUNC_TAIL()                               \
  fci.size = sizeof(fci);                                       \
  fci.function_table = EG(function_table);                      \
  fci.function_name = &function_name;                           \
  fci.symbol_table = NULL;                                      \
  fci.object_ptr = NULL;                                        \
  fci.retval_ptr_ptr = &ret;                                    \
  fci.param_count = param_count;                                \
  fci.params = argv;                                            \
  fci.no_separation = 0;                                        \
  if (zend_call_function(&fci, NULL TSRMLS_CC) == FAILURE) {    \
    ZVAL_BOOL(retval, 0);                                       \
  } else {                                                      \
    if (retval != NULL) {                                       \
      ZVAL_ZVAL(retval, ret, 1, 0);                             \
    }                                                           \
  }                                                             \
  zval_dtor(ret);                                               \
  return;

void bullsoft_call_func3(const char *func, zval *retval, int param_count, zval *param1 = NULL, zval *param2 = NULL, zval *param3 = NULL) {
  BULLSOFT_CALL_FUNC_HEAD();
  
  argv[0] = &param1;
  argv[1] = &param2;
  argv[2] = &param3;
  
  BULLSOFT_CALL_FUNC_TAIL();
}

void bullsoft_call_func2(const char *func, zval *retval, int param_count, zval *param1 = NULL, zval *param2 = NULL) {
  BULLSOFT_CALL_FUNC_HEAD();
  
  argv[0] = &param1;
  argv[1] = &param2;
  
  BULLSOFT_CALL_FUNC_TAIL();
}
  
void bullsoft_call_func1(const char *func, zval *retval, int param_count, zval* param1 = NULL) {
  BULLSOFT_CALL_FUNC_HEAD();
  
  argv[0] = &param1;
  
  BULLSOFT_CALL_FUNC_TAIL();
}


#endif
