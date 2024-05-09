g++ main.cpp -o sfml -lsfml-graphics -lsfml-window -lsfml-system
# if the first command is successful, run the executable
if [ $? -eq 0 ]; then
    ./sfml
fi