executeall:
	g++ -std=c++14 ./source/Main.cpp ./source/View.cpp ./source/Controller.cpp ./source/Shader.cpp ./source/Model.cpp ./source/Camera.cpp ./source/FastTrackball.cpp ./source/SceneNode.cpp ./source/LightNode.cpp ./source/SpotLightNode.cpp ./source/CameraNode.cpp ./source/Animation.cpp -w -lGL -lglfw -lGLEW -o ./bin/modelload
	./bin/modelload
compile:
	g++ -std=c++14 ./source/Main.cpp ./source/View.cpp ./source/Controller.cpp ./source/Shader.cpp ./source/Model.cpp ./source/Camera.cpp ./source/FastTrackball.cpp ./source/SceneNode.cpp ./source/LightNode.cpp ./source/SpotLightNode.cpp ./source/CameraNode.cpp ./source/Animation.cpp -w -lGL -lglfw -lGLEW -o ./bin/modelload
run:
	./bin/modelload
