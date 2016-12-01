DIRS=$(kernelspace userspace)

subdirs :
	${MAKE} -C kernelspace
	${MAKE} -C userspace
