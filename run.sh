g++ main.cpp -o sfml -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# if the first command is successful, run the executable
if [ $? -eq 0 ]; then
    ./sfml
fi