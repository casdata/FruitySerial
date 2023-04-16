//
// Created by castdata on 4/15/2023.
//

#ifndef FRUITYSERIAL_UNDOREDO_H
#define FRUITYSERIAL_UNDOREDO_H


#include "FunctionTools.h"
#include "DTypes.h"
#include <vector>
#include <iostream>
#include <string>

class UndoRedo {
public:
    explicit UndoRedo();

    void addNewUndoRedoData(UndoRedoData* urData);
    void addNewBeforeUndo(UndoRedoData* urData);
    void updateUndoRedoData(const std::string strLine, float caretPosX, float caretIndex);

    UndoRedoData* undo();
    UndoRedoData* redo();

    int getLastCaretIndex();

    bool isUndoAvailable();
    bool isRedoAvailable();
    bool onLastIndex();

    virtual ~UndoRedo();
private:

    int pc;
    std::vector<UndoRedoData*>   undoRedoDataList;

};


#endif //FRUITYSERIAL_UNDOREDO_H
