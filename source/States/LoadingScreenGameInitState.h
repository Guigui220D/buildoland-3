#pragma once

#include "LoadingScreenState.h"

template <class T>
class LoadingScreenGameInitState : public LoadingScreenState<T>
{
    public:
        template <typename... Args>
        LoadingScreenGameInitState(bool fade_in, bool fade_out, Game* game, unsigned int id, Args&&... args);
        ~LoadingScreenGameInitState();

    private:
        void task() override;
};

template <class T>
template <typename... Args>
LoadingScreenGameInitState<T>::LoadingScreenGameInitState(bool fade_in, bool fade_out, Game* game, unsigned int id, Args&&... args) :
    LoadingScreenState<T>(fade_in, fade_out, game, id, args...)
{
    //ctor
}

template <class T>
LoadingScreenGameInitState<T>::~LoadingScreenGameInitState()
{
    //dtor
}

template <class T>
void LoadingScreenGameInitState<T>::task()
{
    //We can access that because Game has LoadingScreenGameInitState as a friend
    this->getGame()->gameBlocksManager.initBlocks();
    this->getGame()->gameGroundsManager.initGrounds();
}
