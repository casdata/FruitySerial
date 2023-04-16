//
// Created by castdata on 4/15/2023.
//

#include "UndoRedo.h"

UndoRedo::UndoRedo() {
    pc = -1;
}

void UndoRedo::addNewUndoRedoData(UndoRedoData *urData) {

    pc++;

    if(pc <= undoRedoDataList.size()) {
        std::vector<UndoRedoData*>::iterator it;

        for(it = undoRedoDataList.begin() + pc; it != undoRedoDataList.end(); it++){

            (*it)->dataLine.clear();
            delete *it;
        }

        undoRedoDataList.erase(undoRedoDataList.begin() + pc, undoRedoDataList.end());
    }

    undoRedoDataList.push_back(urData);

    if(undoRedoDataList.size() > 25){
        pc--;

        undoRedoDataList.at(0)->dataLine.clear();
        delete undoRedoDataList.at(0);

        undoRedoDataList.erase(undoRedoDataList.begin());
    }

     std::cout<<"new -> "<<pc<<" "<<undoRedoDataList.size()<<std::endl;
}

void UndoRedo::addNewBeforeUndo(UndoRedoData *urData) {
    int posIndex = undoRedoDataList.size() - 1;

    if(undoRedoDataList.at(posIndex)->posIndex != urData->posIndex &&
        undoRedoDataList.at(posIndex)->caretPos != urData->caretPos){

        addNewUndoRedoData(urData);
    }
}


void UndoRedo::updateUndoRedoData(const std::string strLine, float caretPosX, float caretIndex) {
    std::cout<<"update -> "<<pc<<" "<<undoRedoDataList.size()<<std::endl;

    undoRedoDataList.at(pc)->dataLine = strLine;
    undoRedoDataList.at(pc)->caretPos = caretPosX;
    undoRedoDataList.at(pc)->posIndex = caretIndex;


}


UndoRedoData *UndoRedo::undo() {
    pc--;

    if(pc < 0) {
        if(undoRedoDataList.empty())
            pc = -1;
        else
            pc = 0;
        return nullptr;
    }

    return undoRedoDataList.at(pc);
}

UndoRedoData *UndoRedo::redo() {
    pc++;

    if(pc >= undoRedoDataList.size()) {
        pc = undoRedoDataList.size() - 1;
        return nullptr;
    }

    return undoRedoDataList.at(pc);
}

int UndoRedo::getLastCaretIndex() {
    if(undoRedoDataList.empty())
        return -1;
    else
        return undoRedoDataList.at(pc)->posIndex;
}

UndoRedo::~UndoRedo() {

    if(!undoRedoDataList.empty()){
        for(auto* urData : undoRedoDataList){
            (*urData).dataLine.clear();
            delete urData;
        }
    }

}

bool UndoRedo::isUndoAvailable() {
    return pc == 0 ? false : true;
}

bool UndoRedo::isRedoAvailable() {
    return pc == (undoRedoDataList.size() - 1) ? false : true;
}

bool UndoRedo::onLastIndex() {
    return pc == undoRedoDataList.size() - 1 ? true : false;
}






