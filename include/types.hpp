#ifndef TYPES_HPP
#define TYPES_HPP

typedef enum {TYPE_REGISTER,TYPE_ADDRESS,TYPE_LABEL,TYPE_NUMBER,TYPE_ANYTHING,TYPE_ADDRESSING,TYPE_TOTAL} e_type;

/**
  * determina se A eh um subtipo de B
  */
bool isSubtype(e_type A, e_type B);

#endif // TYPES_HPP

