all: demo_simple demo demo2 demo3 

demo_simple:
	g++ -I../include demo_simple.cpp -Wfatal-errors -o nn_demo_simple

demo:
	g++ -I../include demo.cpp -Wfatal-errors -o nn_demo
	g++ -DNDEBUG -I../include demo.cpp -Wfatal-errors -o nn_demo_release

demo2:
	g++ -O3 -g -I../include -fno-exceptions demo2.cpp -Wfatal-errors -o nn_demo2
	g++ -O3 -DNDEBUG -g -I../include -fno-exceptions demo2.cpp -Wfatal-errors -o nn_demo2_release

demo3:
	g++ -O3 -S -fno-exceptions          -I../include demo3.cpp -Wfatal-errors -o nn_demo3.s
	g++ -O3 -S -fno-exceptions -DNDEBUG -I../include demo3.cpp -Wfatal-errors -o nn_demo3_release.s

clean:
	rm -rf ./nn_*
