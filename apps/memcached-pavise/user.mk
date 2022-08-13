#EXTRA_CFLAGS = -g -I/home/nvm-admin/pavise/pmdk-1.10/include -I/home/nvm-admin/pavise/include -L/home/nvm-admin/pavise/build/lib -Wno-error=unused-command-line-argument -fexperimental-new-pass-manager -pavise=pavisenoload
EXTRA_CFLAGS = -g -I${PAVISE_ROOT}/include -L${PAVISE_ROOT}/build/lib -Wno-error=unused-command-line-argument -fexperimental-new-pass-manager -pavise=pavisenoload
EXTRA_LDFLAGS = -lpavise_interface
