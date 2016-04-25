#ifndef JS_INCLUDE_H_
#define JS_INCLUDE_H_

#define declare_var_begin(b,a) var a; class var_init_##a { public: var_init_##a :: var_init_##a (){
#define declare_var_end(a) }}; static var_init_##a var_init_##a;

#endif /* JS_INCLUDE_H_ */