bin_PROGRAMS =

######################################################

bin_PROGRAMS += server
server_CFLAGS = \
	-Wall \
	-Wextra \
	-Werror \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers

server_SOURCES = \
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
	-Wno-missing-field-initializers

client_SOURCES = \
	client_src/client.c

client_LDADD = \
	-lcrypto \
	-lz \
	-ldl