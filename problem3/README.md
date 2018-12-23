#### subtask1

##### Compile
```bash
cd subtask1
make
```
##### Run 
```bash
./a.out
```

#### subtask2

##### Compile
```bash
cd subtask2
git submodule update
mkdir -p build
cd build
cmake ..
make
cd ..
```

##### Run 
```bash
./build/src/mall 
```

##### Run tests
```bash
./build/test/mall_test
```


    