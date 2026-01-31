#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }
    void saveScore(int newScore);
    int getHighScore();

    void tick();
protected:
    ModelListener* modelListener;
    int highScore;
};

#endif // MODEL_HPP
