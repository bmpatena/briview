#ifndef SCENEIO_H
#define SCENEIO_H

class sceneIO
{
public:
    sceneIO();
    sceneIO(const string & filename );

    ~sceneIO();

private:
    ofstream* fin;
    ifstream* fout;


};

#endif // SCENEIO_H
