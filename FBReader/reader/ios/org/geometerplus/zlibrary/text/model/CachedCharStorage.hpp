//
//  CachedCharStorage.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/5.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef CachedCharStorage_hpp
#define CachedCharStorage_hpp

#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
class CachedCharStorage
{
private :
    std::string myDirectoryName;
    std::string myFileExtension;
    std::string fileName(int index) {
        std::string res = myDirectoryName;
        char string2[32]={0};
       // itoa(index, string2, 10);
        sprintf(string2, "%d",index);
        res = res+string2;
        res = res +myFileExtension;
        return res;
       // return myDirectoryName + index + myFileExtension;
    }
    
    protected:
    std::vector<char*> myArray;
    std::vector<int> myLengthArray;
public:
    int size() {
        return myArray.size();
    }
    char* block(int index,int* outlen) {
        if (index < 0 || index >= myArray.size()) {
            return 0;
        }
        char* block = myArray[index];
        
        if (block == 0) {
            
            FILE* f = ::fopen(fileName(index).c_str(), "r");
            if(f != 0) {
                
                long pos = ftell(f);
                fseek(f, 0, SEEK_END);
                long size = ftell(f);
                fseek(f, pos, SEEK_SET);
                
      
                if(size >0) {
                    block = new char[size];
                   int cnt =  ::fread(block, 1, size, f);
                    
                    ::fclose(f);
                    //myArray.insert(myArray.end(), block);
                    myArray[index] = block;
                    myLengthArray[index] =cnt;
                    *outlen = cnt;
                }
                
            }
            
            /*try {
                File file = new File(fileName(index));
                int size = (int)file.length();
                if (size < 0) {
                    throw new CachedCharStorageException(exceptionMessage(index, "size = " + size));
                }
                block = new char[size / 2];
                InputStreamReader reader =
                new InputStreamReader(
                                      new FileInputStream(file),
                                      "UTF-16LE"
                                      );
                final int rd = reader.read(block);
                if (rd != block.length) {
                    throw new CachedCharStorageException(exceptionMessage(index, "; " + rd + " != " + block.length));
                }
                reader.close();
            } catch (IOException e) {
                throw new CachedCharStorageException(exceptionMessage(index, null), e);
            }
            myArray.set(index, new WeakReference<char[]>(block));*/
        }
        else {
            *outlen = myLengthArray[index];
        }
        return block;
    }
    CachedCharStorage(std::string directoryName, std::string fileExtension, int blocksNumber) {
        myDirectoryName = directoryName + '/';
        myFileExtension = '.' + fileExtension;
        //myArray.addAll(Collections.nCopies(blocksNumber, new WeakReference<char[]>(null)));
        char * buffer = 0;
        for(int i=0;i<blocksNumber;i++){
           myArray.insert(myArray.end(), buffer);
            myLengthArray.insert(myLengthArray.end(), 0);
        }
        
        
    }
    
};
#endif /* CachedCharStorage_hpp */
