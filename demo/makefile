all: demo_simple demo demo2 demo3 

demo_simple:
	g++ -I../src demo_simple.cpp -Wfatal-errors -o nn_demo_simple

demo:
	g++ -I../src demo.cpp -Wfatal-errors -o nn_demo
	g++ -DNDEBUG -I../src demo.cpp -Wfatal-errors -o nn_demo_release

demo2:
	g++ -O3 -g -I../src -fno-exceptions demo2.cpp -Wfatal-errors -o nn_demo2
	g++ -O3 -DNDEBUG -g -I../src -fno-exceptions demo2.cpp -Wfatal-errors -o nn_demo2_release

demo3:
	g++ -O3 -S -fno-exceptions          -I../src demo3.cpp -Wfatal-errors -o nn_demo3.s
	g++ -O3 -S -fno-exceptions -DNDEBUG -I../src demo3.cpp -Wfatal-errors -o nn_demo3_release.s

clean:
	rm -rf ./nn_*
