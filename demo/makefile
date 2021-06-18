all: demo demo2

demo:
	g++ -I../src demo.cpp -Wfatal-errors -o nn_demo
	g++ -DNDEBUG -I../src demo.cpp -Wfatal-errors -o nn_demo_release

demo2:
	g++ -O3 -g -I../src -fno-exceptions demo2.cpp -Wfatal-errors -o nn_demo2
	g++ -O3 -DNDEBUG -g -I../src -fno-exceptions demo2.cpp -Wfatal-errors -o nn_demo2_release

clean:
	rm -rf ./nn_*
