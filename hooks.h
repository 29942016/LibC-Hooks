#ifndef HOOKS_H_INCLUDED
#define HOOKS_H_INCLUDED

extern asmlinkage int (*original_uname) (struct new_utsname *);
extern asmlinkage int overide_uname(struct new_utsname *buf);

extern asmlinkage int (*original_open) (char* file, int flag, int mode);
extern asmlinkage int overide_open(char* file, int flag, int mode);

#endif
