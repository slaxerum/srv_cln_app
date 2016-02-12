bin_PROGRAMS =

######################################################

bin_PROGRAMS += server
server_CFLAGS = \
	-Wall \
	-Wextra \
	-Werror \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-I$(srcdir)/common_src

server_SOURCES = \
	common_src/log.c \
	server_src/server.c

server_LDADD = \
	-lcrypto \
	-lz \
	-ldl
		
bin_PROGRAMS += client
client_CFLAGS = \
	-Wall \
	-Wextra \
	-Werror \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-I$(srcdir)/common_src

client_SOURCES = \
	client_src/usage.c \
	common_src/log.c \
	client_src/client.c

client_LDADD = \
	-lcrypto \
	-lz \
	-ldl