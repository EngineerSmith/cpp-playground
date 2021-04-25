FILEPATH="/storage/emulated/0/code/cpp"
echo "Compiling..."
if clang++ $FILEPATH/main.cpp -lm -o $HOME/cpp; then
chmod +x $HOME/cpp
echo "Successful! Now running..."
echo "------"
$HOME/cpp
else
echo "Failed to Compile"
fi