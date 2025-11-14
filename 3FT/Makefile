all: ft
clean:
	rm -f ft meminfo*.out
clobber: clean
	rm -f dynarray.o path.o nodeFT.o ft_client.o ft.o *~


ft: dynarray.o path.o nodeFT.o ft_client.o, ft.o
	gcc217 -g $^ -o $@

dynarray.o: dynarray.c dynarray.h
	gcc217 -g -c $<

path.o: path.c path.h
	gcc217 -g -c $<

nodeFT.o: nodeFT.c nodeFT.h path.c path.h dynarray.c dynarray.h a4def.h
	gcc217 -g -c $<

ft_client.o: ft_client.c ft.c ft.h dynarray.c dynarray.h nodeFT.c nodeFT.h a4def.h
	gcc217 -g -c $<

ft.o: ft.c ft.h nodeFT.c nodeFT.h dynarray.c dynarray.h a4def.h
	gcc217 -g -c $<