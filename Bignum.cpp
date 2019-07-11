#include <vector>
#include <iostream>
using namespace std;


class Bignum{
    public:
        vector<int>num; //  从后向前记录数字
        bool tag;       //  true为正数  false为负数
        void trim(void);//  去掉前置的零，并将(-0)变为正数
        //  初始化函数
        Bignum(int);
        Bignum(string);
        Bignum(const Bignum &);
        Bignum(void);
        //  打印数字
        void print(int n = 0);
        //  打印const类型的Bignum
        friend void const_print(const Bignum &);
        //  重载赋值等于号
        Bignum operator = (const Bignum &);
        //  重载负号
        friend Bignum operator - (const Bignum &);
        //  重载比较运算符
        friend bool operator == (const Bignum &, const Bignum &);
        friend bool operator != (const Bignum &, const Bignum &);
        friend bool operator < (const Bignum &, const Bignum &);
        friend bool operator <= (const Bignum &, const Bignum &);
        friend bool operator > (const Bignum &, const Bignum &);
        friend bool operator >= (const Bignum &, const Bignum &);
        //  重载计算运算符
        friend Bignum operator + (const Bignum &, const Bignum &);
        friend Bignum operator += (Bignum &, const Bignum &);
        friend Bignum operator - (const Bignum &, const Bignum &);
        friend Bignum operator -= (Bignum &, const Bignum &);
        friend Bignum operator * (const Bignum &, const Bignum &);
        friend Bignum operator *= (Bignum &, const Bignum &);
        friend Bignum operator / (const Bignum &, const Bignum &);
        friend Bignum operator /= (Bignum &, const Bignum &);
        friend Bignum operator % (const Bignum &, const Bignum &);
        friend Bignum operator %= (Bignum &, const Bignum &);
};

//  去掉前置的零并把(-0)变成0
void Bignum::trim(void){
    vector<int>::reverse_iterator rit = num.rbegin();
    while((*rit) == 0 && rit != num.rend()){
        num.pop_back();
        rit = num.rbegin();
    }
    if(num.empty()){
        num.push_back(0);
        tag = true;
    }
}

//  初始化
Bignum::Bignum(int val){
    if(val >= 0){
        tag = true;
    }
    else{
        tag = false;
        val *= -1;
    }
    while(val){
        num.push_back(val%10);
        val /= 10;
    }
}

Bignum::Bignum(string val){
    tag = true;
    for(string::reverse_iterator rit = val.rbegin(); rit != val.rend(); rit++){
        if((*rit) == '-'){
            tag = false;
            break;
        }
        else{
            num.push_back((*rit)-'0');
        }
    }
    trim();
}

Bignum::Bignum(const Bignum & x){
    tag = x.tag;
    num = x.num;
}

Bignum::Bignum(void){
    tag = true;
    num.push_back(0);
}

//  打印数字
void Bignum::print(int n){
    if(!tag)    cout << '-';
    for(vector<int>::reverse_iterator rit = num.rbegin(); rit != num.rend(); rit++){
        cout << *rit;
    }
    //  增加n只是为了调试时美观
    if(n){
        cout << endl;
    }
}

void const_print(const Bignum & x){
    if(!x.tag)   cout << '-';
    for(vector<int>::const_reverse_iterator crit = x.num.rbegin(); crit != x.num.rend(); crit++){
        cout << (*crit);
    }
}

//  重载复制等于号
Bignum Bignum::operator = (const Bignum & x){
    num = x.num;
    tag = x.tag;
    return *this;
}

//  重载负号
Bignum operator - (const Bignum & x){
    Bignum temp(x);
    temp.tag = !temp.tag;
    return temp;
}

//  重载等于号
bool operator == (const Bignum & x, const Bignum & y){
    //  如果异号或者长度不同(前提是我们在运算过程中不存在前置零)
    if(x.tag != y.tag || x.num.size() != y.num.size()){
        return false;
    }
    //  按位进行比较
    vector<int>::const_iterator xit, yit;
    xit = x.num.begin();
    yit = y.num.begin();
    while(xit != x.num.end()){
        if((*xit) != (*yit)){
            return false;
        }
        xit++;
        yit++;
    }
    //  如果每位都相同就返回真
    return true;
}

//  重载不等号
bool operator != (const Bignum & x, const Bignum & y){
    return !(x == y);
}

//  重载小于号
bool operator < (const Bignum & x, const Bignum & y){
    //  同号
    if(x.tag == y.tag){
        //  均为正数
        if(x.tag){
            if(x.num.size() == y.num.size()){
                vector<int>::const_reverse_iterator xrit, yrit;
                xrit = x.num.rbegin();
                yrit = y.num.rbegin();
                while(xrit != x.num.rend()){
                    if((*xrit) != (*yrit)){
                        return (*xrit) < (*yrit);
                    }
                    xrit++;
                    yrit++;
                }
                //  到达终点一直相同
                return false;
            }
            //  长度不同
            else{
                return x.num.size() < y.num.size();
            }
        }
        //  均为负数
        else{
            return !((-x) < (-y));
        }
    }
    //  异号
    else{
        return x.tag < y.tag;
    }
}

bool operator <= (const Bignum & x, const Bignum & y){
    return ((x < y) || (x == y));
}

bool operator > (const Bignum & x, const Bignum & y){
    return !(x <= y);
}

bool operator >= (const Bignum & x, const Bignum & y){
    return !(x < y);
}

//  重载加号(调用+=)
Bignum operator + (const Bignum & x, const Bignum & y){
    Bignum temp(x);
    return temp += y;
}

//  重载加等(只处理两个正数相加)
Bignum operator += (Bignum & x, const Bignum & y){
    //  同号
    if(x.tag == y.tag){
        bool to_add = 0;
        vector<int>::iterator xit = x.num.begin();
        vector<int>::const_iterator yit = y.num.begin();
        while(xit != x.num.end() && yit != y.num.end()){
            (*xit) += (*yit) + to_add;
            //  处理进位
            if((*xit) > 9){
                to_add = true;
                (*xit) -= 10;
            }
            else{
                to_add = false;
            }
            xit++;
            yit++;
        }
        while(xit != x.num.end()){
            (*xit) = (*xit) + to_add;
            if((*xit) > 9){
                to_add = true;
                (*xit) -= 10;
            }
            else{
                to_add = false;
            }
            xit++;
        }
        while(yit != y.num.end()){
            int buf = (*yit) + to_add;
            if(buf > 9){
                to_add = true;
                buf -= 10;
            }
            else{
                to_add = false;
            }
            yit++;
            x.num.push_back(buf);
        }
        if(to_add){
            x.num.push_back(1);
        }
        return x;
    }
    //  异号
    else{
        //  x为正数
        if(x.tag){
            return (x - y);
        }
        // x为负数
        else{
            return (y - x);
        }
    }
}

//  重载减法
Bignum operator - (const Bignum & x, const Bignum & y){
    Bignum temp(x);
    return (temp -= y);
}

//  重载减等(只处理两个正数相减，且x>y)
Bignum operator -= (Bignum & x, const Bignum & y){
    //  同号
    if(x.tag == y.tag){
        //  均为正数
        if(x.tag){
            if(x >= y){
                bool to_sub = false;
                vector<int>::iterator xit = x.num.begin();
                vector<int>::const_iterator yit = y.num.begin();
                while(yit != y.num.end()){
                    (*xit) -= (*yit) + to_sub;
                    if((*xit) < 0){
                        to_sub = true;
                        (*xit) += 10;
                    }
                    else{
                        to_sub = false;
                    }
                    xit++;
                    yit++;
                }
                while(xit != x.num.end()){
                    (*xit) -= to_sub;
                    if((*xit) < 0){
                        to_sub = true;
                        (*xit) += 10;
                    }
                    else{
                        to_sub = false;
                    }
                    xit++;
                }
                x.trim();
                return x;
            }
            //  x<y
            else{
                x = -(y-x);
                x.trim();
                return x;
            }
        }
        //  均为负数
        else{
            if(x >= y){
                x = (-y)-(-x);
                x.trim();
                return x;
            }
            else{
                x = -((-x)-(-y));
                x.trim();
                return x;
            }
        }
    }
    //  异号
    else{
        if(x > Bignum(0)){
            x = (x + (-y));
            x.trim();
            return x;
        }
        else{
            x = -((-x) + y);
            x.trim();
            return x;
        }
    }
}

//  重载乘号
Bignum operator * (const Bignum & x, const Bignum & y){
    Bignum temp(x);
    return (temp *= y);
}

//  重载乘等号
Bignum operator *= (Bignum & x, const Bignum & y){
    //  同号
    if(x.tag == y.tag){
        //  均为正数
        if(x.tag){
            Bignum product, product_buf;
            //  若两个数有任何一个为0，则返回0
            if(x == product || y == product){
                return product;
            }
            //  前置乘十计数
            int add_zero = 0;
            //  to_add ： 进位  buf : 每位数
            int to_add = 0, buf = 0;
            for(vector<int>::const_iterator yit = y.num.begin(); yit != y.num.end(); yit++){
                //  清空buf的vector
                product_buf.num.erase(product_buf.num.begin(), product_buf.num.end());
                //  插入0
                product_buf.num.insert(product_buf.num.begin(), add_zero++, 0);
                //  模拟手算
                for(vector<int>::iterator xit = x.num.begin(); xit != x.num.end(); xit++){
                    buf = (*xit) * (*yit) + to_add;
                    to_add = buf / 10;
                    buf %= 10;
                    product_buf.num.push_back(buf);
                }
                if(to_add){
                    product_buf.num.push_back(to_add);
                    to_add = 0;
                }
                product += product_buf;
                product_buf = Bignum(0);
            }
            x = product;
            return x;
        }
        //  均为负数
        else{
            x = ((-x) * (-y));
            return x;
        }
    }
    //  异号
    else{
        //  x为正数
        if(x.tag){
            x =  -(x * (-y));
            return x;
        }
        //  x为负数
        else{
            x = -((-x) * y);
            return x;
        }
    }
}

//  重载除号
Bignum operator / (const Bignum & x, const Bignum & y){
    Bignum temp(x);
    return (temp /= y);
}

//  重载除等号
Bignum operator /= (Bignum & x, const Bignum & y){
    //  特判x和y为零的情况
    if(x == Bignum(0)){
        return x;
    }
    if(y == Bignum(0)){
        //  这里应该报错
        cout << "被除数不应为0!" << endl;
        x = 0;
        return x;
    }
    //  同号
    if(x.tag == y.tag){
        //  同为正数
        if(x.tag){
            Bignum ans;
            if(x.num.size() < y.num.size()){
                x = ans;
                return x;
            }
            else{
                Bignum div_buf;
                vector<int>::iterator it = x.num.end()-y.num.size();
                div_buf.num.assign(it, x.num.end());
                while(it != x.num.begin()){
                    if(div_buf < y){
                        it--;
                        //  已经整除
                        div_buf.num.insert(div_buf.num.begin(), (*it));
                        div_buf.trim();
                        ans *= 10;
                    }
                    while(div_buf >= y){
                        div_buf -= y;
                        ans += 1;
                    }
                }
                while(div_buf >= y){
                    div_buf -= y;
                    ans += 1;
                }
                x = ans;
                return x;
            }
        }
        //  同为负数
        else{
            x = ((-x) / (-y));
            return x;
        }
    }
    //  异号
    else{
        //  x为正数
        if(x.tag){
            x = -(x / (-y));
            x.trim();
            return x;
        }
        //  x为负数
        else{
            x = -((-x) / y);
            x.trim();
            return x;
        }
    }
}

//  重载求余号
Bignum operator % (const Bignum & x, const Bignum & y){
    Bignum temp(x);
    return (temp %= y);
}

//  重载模等号
Bignum operator %= (Bignum & x, const Bignum & y){
    //  特判x和y为零的情况
    if(x == Bignum(0)){
        return x;
    }
    if(y == Bignum(0)){
        //  这里应该报错
        cout << "被除数不应为0!" << endl;
        x = 0;
        return x;
    }
    //  同号
    if(x.tag == y.tag){
        //  同为正数
        if(x.tag){
            Bignum ans;
            if(x.num.size() < y.num.size()){
                return x;
            }
            else{
                Bignum div_buf;
                vector<int>::iterator it = x.num.end()-y.num.size();
                div_buf.num.assign(it, x.num.end());
                while(it != x.num.begin()){
                    if(div_buf < y){
                        it--;
                        //  已经整除
                        div_buf.num.insert(div_buf.num.begin(), (*it));
                        div_buf.trim();
                        ans *= 10;
                    }
                    while(div_buf >= y){
                        div_buf -= y;
                        ans += 1;
                    }
                }
                while(div_buf >= y){
                    div_buf -= y;
                    ans += 1;
                }
                div_buf.trim();
                x = div_buf;
                return x;
            }
        }
        //  同为负数
        else{
            x = ((-x) % (-y));
            return x;
        }
    }
    //  异号
    else{
        //  x为正数
        if(x.tag){
            x = -(x % (-y));
            x.trim();
            return x;
        }
        //  x为负数
        else{
            x = -((-x) % y);
            x.trim();
            return x;
        }
    }
}

int main(void){
    //  基本的初始化
    Bignum A(10), B("0020"), C;   
    A.print(1); //  10
    B.print(1); //  20
    C.print(1); //  0
    Bignum D(B);
    D.print(1); //  20

    //  赋值操作
    Bignum E = D;
    E.print(1); //  20

    // 负号+赋值
    Bignum F = -A;
    F.print(1); //  -10

    //  比较
    cout << (D == B) << endl;   //  1
    cout << (A != B) << endl;   //  1
    cout << (A < B) << endl;    //  1
    cout << (F > A) << endl;    //  0
    cout << (A <= A) << endl;   //  1
    cout << (A >= A) << endl;   //  1

    //  四则运算
    (A+B).print(1); //  10+20 => 30
    (A-B).print(1); //  10-20 => -10
    (A*B).print(1); //  10*20 => 200
    (A/B).print(1); //  10/20 => 0
    (A%B).print(1); //  10%20 => 10
    system("pause");
    return 0;
}
