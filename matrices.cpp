#ifndef __MATRICES_H_

#define __MATRICES_H_

#include<iostream>
#include<iomanip>
#include<vector>
#include<math.h>
using namespace std;

template <class T>
class Matrix{
private:
    int r;
    int c;
    vector<vector<T>> m;
public:
    Matrix(void):r(0),c(0){}
    Matrix(int _r,int _c,T fill=0):r(_r),c(_c){
        for (int i=0;i<r;++i){
            m.push_back(vector<T>(c,fill));
        }
    } 
    Matrix(vector<vector<T>> &_m){
        r = _m.size();
        c = _m[0].size();
        m.assign(_m.begin(),_m.end());
    }
    Matrix(vector<T> &_m,int _r=1):r(_r){
        c = _m.size()/r;

        // note that the ``typename`` in the front is necessary
        typename vector<T>::iterator top = _m.begin();

        for (int i=0;i<r;++i){
            m.push_back(vector<T>(0));
            for (int j=0;j<c;++j){
                m[i].push_back(*(top++));
            }
        }
    }
    
    int height(void);
    int width(void);
    T& entry(int x,int y);
    vector<vector<T>>& self(void);
    void print(void);
    //Matrix<T>& operator=(Matrix<T> &_m);
    Matrix<T>& operator=(vector<vector<T>> &_m);
    Matrix<T>& operator+=(Matrix<T> &_m);
    Matrix<T>& operator-=(Matrix<T> &_m);
    Matrix<T>& operator*=(Matrix<T> &_m);
    Matrix<T>& operator*=(T _m);
    Matrix<T>& transpose(void);
    Matrix<T>& impose(T(*function)(T)); 
    double frobenius(void);



    // note that it needs an extra (and different) template <class X> to declare a friend
    template <class T2> friend Matrix<T2>& ToMatrix(vector<vector<T2>> &_m);
    template <class T2> friend Matrix<T2>& ToMatrix(vector<T2> &_m,int _r);

    template <class T2> friend Matrix<T2>& operator+(Matrix<T2> &_m1,Matrix<T2> &_m2);
    template <class T2> friend Matrix<T2>& operator-(Matrix<T2> &_m1,Matrix<T2> &_m2);
    template <class T2> friend Matrix<T2>& operator*(Matrix<T2> &_m1,Matrix<T2> &_m2);
    template <class T2> friend Matrix<T2>& operator*(Matrix<T2> &_m1,T2 _m2);
    template <class T2> friend Matrix<T2>& operator*(T2 _m1,Matrix<T2> &_m2);

    template <class T2> friend Matrix<T2>& Sum(Matrix<T2> &_m1,int flag);
    template <class T2> friend Matrix<T2>& Dot(Matrix<T2> &_m1,Matrix<T2> &_m2);
    template <class T3,class T2> friend Matrix<T3>& Impose(Matrix<T2> &_m1,T3(*function)(T2));
    template <class T2> friend Matrix<T2>& Pile(Matrix<T2> &_m1,int,int);
public:
};


template <class T>
int Matrix<T> :: height(void){
    return r;
}

template <class T>
int Matrix<T> :: width(void){
    return c;
}

template <class T>
T& Matrix<T> :: entry(int x,int y){
    return m[x][y];
}

template <class T>
vector<vector<T>>& Matrix<T> :: self(void){
    return m;
}


template <class T>
void Matrix<T> :: print(void){
    if (r<=0 || c<=0){
        cout << "[[]]\n";
        return ;
    }
    cout << '[';
    const int setwidth = 8;  // const int setwidth = 13;
    for (int i=0;i<r-1;++i){
        if (i) cout << ' ';
        cout << '['; 
        for (int j=0;j<c-1;++j){
            // please #include <iomanip> when using setw
            cout << setw(setwidth) << m[i][j] << ',';
        }
        cout << setw(setwidth) << m[i][c-1] << "]\n";
    }
    if (r>1) cout << " ["; else cout << '[';
    for (int j=0;j<c-1;++j){
            cout << setw(setwidth) << m[r-1][j] << ',';
    }
    cout << setw(setwidth) << m[r-1][c-1] << "]]\n";
    return ;
}


template <class T>
Matrix<T>&  Matrix<T> :: transpose(void){
    Matrix<T>* new_matrix = new Matrix<T>(c,r);
    for (int i=c-1;i>=0;--i){
        for (int j=r-1;j>=0;--j){
            new_matrix->entry(i,j) = m[j][i];
        }
    }
    return *new_matrix;
}


template <class T> 
Matrix<T>& Matrix<T> :: impose(T(*function)(T)){
    for (int i=r-1;i>=0;--i){
        for (int j=c-1;j>=0;--j){
            m[i][j] = function(m[i][j]);
        }
    }
    return *this;
}


template <class T>
double Matrix<T> :: frobenius(void){
    double sum = 0;
    for (int i=r-1;i>=0;--i){
        for (int j=c-1;j>=0;--j){
            sum += m[i][j] * m[i][j];
        }
    }
    return sqrt(sum); 
}



template <class T>
Matrix<T>& ToMatrix(vector<vector<T>> &_m){
    Matrix<T>* new_matrix = new Matrix<T> (_m);
    return *new_matrix;
}

template <class T>
Matrix<T>& ToMatrix(vector<T> &_m,int _r = 1){
    Matrix<T>* new_matrix = new Matrix<T> (_m,_r);
    return *new_matrix;
}


/*
template <class T>
Matrix<T>& Matrix<T> :: operator=(Matrix<T> &_m){
    r = _m.height();
    c = _m.width();
    m.assign(_m.self().begin(),_m.self().end());
    return *this;
}
*/

template <class T>
Matrix<T>& Matrix<T> :: operator=(vector<vector<T>> &_m){
    r = _m.size();
    c = _m[0].size();
    m.assign(_m.begin(),_m.end());
    return *this;
}


template <class T>
Matrix<T>& operator+(Matrix<T> &_m1,Matrix<T> &_m2){
    Matrix<T>* new_matrix = new Matrix<T> (_m1.height(),_m2.width());
    for (int i=_m1.height()-1;i>=0;--i){
        for (int j=_m2.width()-1;j>=0;--j){
            new_matrix->entry(i,j) = _m1.entry(i,j) + _m2.entry(i,j);
        }
    }
    return *new_matrix;
}

template <class T>
Matrix<T>& Matrix<T> :: operator+=(Matrix<T> &_m1){
    for (int i=_m1.height()-1;i>=0;--i){
        for (int j=_m1.width()-1;j>=0;--j){
            this->entry(i,j) += _m1.entry(i,j);
        }
    }
    return *this;
}


template <class T>
Matrix<T>& operator-(Matrix<T> &_m1,Matrix<T> &_m2){
    Matrix<T>* new_matrix = new Matrix<T> (_m1.height(),_m2.width());
    for (int i=_m1.height()-1;i>=0;--i){
        for (int j=_m2.width()-1;j>=0;--j){
            new_matrix->entry(i,j) = _m1.entry(i,j) - _m2.entry(i,j);
        }
    }
    return *new_matrix;
}

template <class T>
Matrix<T>& Matrix<T> :: operator-=(Matrix<T> &_m1){
    for (int i=_m1.height()-1;i>=0;--i){
        for (int j=_m1.width()-1;j>=0;--j){
            this->entry(i,j) -= _m1.entry(i,j);
        }
    }
    return *this;
}


template <class T>
Matrix<T>& operator*(Matrix<T> &_m1,Matrix<T> &_m2){
    Matrix<T>* new_matrix = new Matrix<T> (_m1.height(),_m2.width());
    for (int i=_m1.height()-1;i>=0;--i){
        for (int j=_m2.width()-1;j>=0;--j){
            new_matrix->entry(i,j) = 0;
            for (int k=0;k<_m1.width();++k){
                new_matrix->entry(i,j) += _m1.entry(i,k) * _m2.entry(k,j);
            }
        }
    }
    return *new_matrix;
}

template <class T>
Matrix<T>& operator*(Matrix<T> &_m1,T _m2){
    Matrix<T>* new_matrix = new Matrix<T> (_m1.height(),_m1.width());
    for (int i=_m1.height()-1;i>=0;--i){
        for (int j=_m1.width()-1;j>=0;--j){
            new_matrix->entry(i,j) = _m1.entry(i,j) * _m2;
        }
    }
    return *new_matrix;
}

template <class T>
Matrix<T>& Matrix<T> :: operator*=(Matrix<T> &_m1){
    return *this = (*this) * _m1;
}



// flag = 1: reduce to a row vector
// flag = 2: reduce to a column vector
template <class T>
Matrix<T>& Sum(Matrix<T> &_m1,int flag){
    Matrix<T>* new_matrix;
    switch (flag){
        case 1:
            new_matrix = new Matrix<T> (1,_m1.width());
            for (int j=_m1.width()-1;j>=0;--j){
                new_matrix->entry(0,j) = _m1.entry(0,j);
                for (int i=_m1.height()-1;i;--i){
                    new_matrix->entry(0,j) += _m1.entry(i,j);
                }
            }
            return *new_matrix;            
        case 2:
            new_matrix = new Matrix<T> (_m1.height(),1);
            for (int i=_m1.height()-1;i>=0;--i){
                new_matrix->entry(i,0) = _m1.entry(i,0);
                for (int j=_m1.width()-1;j;--j){
                    new_matrix->entry(i,0) += _m1.entry(i,j);
                }
            }
            return *new_matrix; 
        //default:
    }
    return *new_matrix;  
}


template <class T>
Matrix<T>& Dot(Matrix<T> &_m1,Matrix<T> &_m2){
    Matrix<T>* new_matrix = new Matrix<T> (_m1.height(),_m1.width());
    for (int i=_m1.height()-1;i>=0;--i){
        for (int j=_m1.width()-1;j>=0;--j){
            new_matrix->entry(i,j) = _m1.entry(i,j) * _m2.entry(i,j);
        }
    }
    return *new_matrix;
}


template <class T2,class T>
Matrix<T2> & Impose(Matrix<T> &_m,T2(*function)(T)){
    Matrix<T2>* new_matrix = new Matrix<T2>(_m.height(),_m.width());
    for (int i=_m.height()-1;i>=0;--i){
        for (int j=_m.width()-1;j>=0;--j){
            new_matrix->entry(i,j) = function(_m.entry(i,j));
        }
    }
    return *new_matrix;
}


template <class T>
Matrix<T> & Pile(Matrix<T> &_m1,int row,int width){
    Matrix<T>* new_matrix = new Matrix<T> (_m1.height()*row,_m1.width()*width);
    for (int i2=new_matrix->height()-_m1.height();i2>=0;i2-=_m1.height()){
        for (int j2=new_matrix->width()-_m1.width();j2>=0;j2-=_m1.width()){
            for (int i=_m1.height()-1;i>=0;--i){
                for (int j=_m1.width()-1;j>=0;--j){
                    new_matrix->entry(i2+i,j2+j) = _m1.entry(i,j);
                }
            }
        }
    }
    return *new_matrix;
}







#endif