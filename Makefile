all: compile link run

compile:
	g++ -c main.cpp -I"C:\\Users\\jacks\\OneDrive\\Desktop\\SFML-2.6.1\\include" -DSFML_STATIC

link:
	g++ main.o -o main -L"C:\\Users\\jacks\\OneDrive\\Desktop\\SFML-2.6.1\\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -lsfml-main

run: 
	.\main
	
clean:
	rm -f main *.o