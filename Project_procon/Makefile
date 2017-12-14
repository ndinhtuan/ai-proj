LIBS = -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
FLAGS = -I/usr/local/include/opencv -I/usr/local/include -I/usr/local/include/opencv2

# tạo file execute testHelper
testHelper:  helper.o Frame.o Piece.o testHelper.o Bot1.o Bot.o Grid.o ValueIndex.o helperPiece.o Dot.o
	g++ --std=c++11 -o testHelper clipper.cpp Bot1.o Bot.o testHelper.o Grid.o Dot.o helperPiece.o ValueIndex.o Frame.o Piece.o helper.o $(LIBS) $(FLAGS)

# tạo file execute testGrid
testGrid: Grid.o helper.o testGrid.o Frame.o Piece.o
	g++ --std=c++11 -o testGrid clipper.cpp testGrid.o Grid.o helper.o Frame.o Piece.o $(LIBS) $(FLAGS)

testPiece: Grid.o helper.o testPiece.o Frame.o Piece.o 
	g++ --std=c++11 -o testPiece clipper.cpp testPiece.o Grid.o helper.o Frame.o Piece.o $(LIBS) $(FLAGS)

testBot: testBot.o Grid.o Piece.o helper.o Frame.o Bot.o ValueIndex.o
	g++ --std=c++11 -o testBot testBot.o Grid.o Bot.o helper.o Frame.o Piece.o $(LIBS) $(FLAGS)

testBot.o: testBot.cpp 
	g++ -c testBot.cpp

Bot1.o : Bot1.cpp Bot1.h 
	g++ --std=c++11 -c Bot1.cpp
Dot.o: Dot.cpp Dot.h 
	g++ -c Dot.cpp

helperPiece.o: helperPiece.h helperPiece.cpp
	g++ -c helperPiece.cpp

ValueIndex.o: ValueIndex.h ValueIndex.cpp
	g++ -c ValueIndex.cpp

Bot.o: Bot.h Bot.cpp 
	g++ --std=c++11 -c Bot.cpp

testPiece.o : testPiece.cpp 
	g++ -c testPiece.cpp

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
	g++ --std=c++11 -c testHelper.cpp

all: testGrid;

clean:
	rm -f testHelper testHelper.o Bot.o Piece.o Frame.o Dot.o helperPiece.o  Grid.o ValueIndex.o helper.o
	

