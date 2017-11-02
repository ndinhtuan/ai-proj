LIBS = -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
FLAGS = -I/usr/local/include/opencv -I/usr/local/include -I/usr/local/include/opencv2

# tạo file execute testHelper
testHelper:  helper.o Frame.o Piece.o testHelper.o  
	g++ --std=c++11 -o testHelper testHelper.o Frame.o Piece.o helper.o

# tạo file execute testGrid
testGrid: Grid.o helper.o testGrid.o Frame.o Piece.o
	g++ --std=c++11 -o testGrid testGrid.o Grid.o helper.o Frame.o Piece.o $(LIBS) $(FLAGS)

testGrid.o : testGrid.cpp
	g++ -c testGrid.cpp

Grid.o : Grid.h Grid.cpp
	g++ -c Grid.cpp

Frame.o: Frame.h Frame.cpp 
	g++ -c Frame.cpp 

Piece.o: Piece.h Piece.cpp 
	g++ -c Piece.cpp

helper.o: helper.h helper.cpp 
	g++ -c helper.cpp

testHelper.o: testHelper.cpp
	g++ -c testHelper.cpp

all: testGrid;

clean:
	rm -f testHelper testHelper.o
	rm -f testGrid testGrid.o

