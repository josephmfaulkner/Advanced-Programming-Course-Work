/*STUDENTS: Joseph Faulkner (jofaulkn) and Megan Muir (memuir)*/

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that): uvalue (that) {
    string that_string = to_string(that);
    //this->ubig_value.push_back();
    for(int i = that_string.length() -1; i >=0 ; i--){
        ubig_value.push_back(that_string[i]);
        DEBUGF ('c', "INSERTED:" << ubig_value.back() )
    }
    DEBUGF ('~', this << " -> " << uvalue << ", " << that_string)


}

ubigint::ubigint (const string& that): uvalue(0) {
    DEBUGF ('c', that)
    for(int i = that.length() -1; i >=0 ; i--){

        ubig_value.push_back(that[i]);

        DEBUGF ('c', "INSERTED:" << ubig_value.back() )
    }

   DEBUGF ('~', "CONSTRUCTED:" << this << " -> " << uvalue)

}

ubigint ubigint::operator+ (const ubigint& that) const {
   ubigint result;
   int carry = 0;
   int sum = 0;
   int digit = 0;
   unsigned char digit_char;
   ubigvalue_t smaller;
   ubigvalue_t larger;
   if (this->ubig_value.size() < that.ubig_value.size()){
        smaller = this->ubig_value;
        larger = that.ubig_value;
   } else {
       larger = this->ubig_value;
       smaller = that.ubig_value;
   }

   for(unsigned int count= 0;
       count < larger.size();
       count ++){
        if (count < smaller.size()){
            sum = ((this->ubig_value.at(count)) - '0') +
                    ((that.ubig_value.at(count)) -'0') +
                    carry;
        }
        else {
            sum = (larger.at(count) - '0') +carry;
        }
        digit = sum % 10;
        carry = sum / 10;
        digit_char = digit + '0';
        result.ubig_value.push_back((digit_char));

   }
   if (carry > 0){
      unsigned char carry_char = carry + '0';
      result.ubig_value.push_back(carry_char);
      };
     return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
     DEBUGF('S', "COMPUTING: "<< *this << " - " << that)
     if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
//   return ubigint (uvalue - that.uvalue);

    ubigint result;
    int carry = 0;
    int digit = 0;
    unsigned char digit_char;
    ubigvalue_t smaller;
    ubigvalue_t larger;

        larger = this->ubig_value;
        smaller = that.ubig_value;

    for (unsigned int count = 0; count < larger.size(); count++ ){

        while (count < smaller.size()){
            if ((larger[count] - 0) >= (smaller[count]) - 0){
                digit = larger[count] - smaller[count] - carry;
                carry = 0;
                digit_char = digit + '0';
            } else {
                digit = (larger[count] + 10) - smaller[count] - carry;
                carry = 1;
                digit_char = digit + '0';
            }
            result.ubig_value.push_back((digit_char));
            count++;
        }
        if (count < larger.size()){
            if ((larger[count]- '0') < carry){
                digit = (larger[count] - '0') + 10 - carry;
                carry = 1;
            } else {
                digit = (larger[count] - '0') - carry;
                carry = 0;
            }
            digit_char = digit + '0';
            result.ubig_value.push_back((digit_char));

        }
        DEBUGF('S', "SUBTRACT RESULT: " <<result)
    }

    DEBUGF('S', "POPOFF: ")
    while(true){
    if((result.ubig_value.back() == '0') &&
            (result.ubig_value.size() > 1)){
        result.ubig_value.pop_back();
        DEBUGF('S', "SUBTRACT RESULT: " <<result)
    }
    else
        break;
    }
    DEBUGF('S', "SUBTRACT RESULT: " <<result)
    return result;


}

ubigint ubigint::operator* (const ubigint& that) const {
   ubigint result;
   ubigvalue_t smaller;
   ubigvalue_t larger;
   int carry = 0;
   int digit = 0;
   unsigned char digit_char;
   if (this->ubig_value.size() < that.ubig_value.size()){
        smaller = this->ubig_value;
        larger = that.ubig_value;
   } else {
       larger = this->ubig_value;
       smaller = that.ubig_value;
   }
   //Initialize result value
   for (unsigned int count = 0;
        count < smaller.size()+ larger.size();
        count++){
       result.ubig_value.push_back('0');

   }

   for(unsigned int i = 0;
       i < larger.size();
       i ++)
   {
       carry = 0;
       for(unsigned int j = 0;
           j < smaller.size();
           j ++)
       {
           digit = (result.ubig_value.at(i+j) - '0') +
                   (larger.at(i) - '0') * (smaller.at(j) - '0') +
                   carry;
           carry = digit / 10;
           digit_char = (digit % 10) + '0';
           result.ubig_value[i+j] = digit_char;

       }
       result.ubig_value[i+smaller.size()] = carry + '0';

   }

       while(true){
       if((result.ubig_value.back() == '0') &&
               (result.ubig_value.size() > 1))
           result.ubig_value.pop_back();
       else
           break;
       }

   return result;
}

void ubigint::multiply_by_2() {
   uvalue *=2;
   ubigint two = 2;
   *this = two * (*this);
}

void ubigint::divide_by_2() {
   uvalue /= 2;

   //int size =this->ubig_value.size();
   int digit_result = 0;
   int next_isodd = 0;
   for (unsigned int i = 0; i < this->ubig_value.size(); i++){
       digit_result = (this->ubig_value.at(i) - '0') / 2;

       if(i < this->ubig_value.size() -1){
         next_isodd = (this->ubig_value.at(i+1) - '0') % 2;

         if(next_isodd){
            digit_result += 5;
         }

       }

       this->ubig_value[i] = digit_result + '0';

   }

   while(true){
   if((this->ubig_value.back() == '0') && (this->ubig_value.size() > 1))
       this->ubig_value.pop_back();
   else
       break;
   }

}


ubigint::quot_rem ubigint::divide (const ubigint& that) const {
   static const ubigint zero = 0;
   if (that == zero) throw domain_error ("ubigint::divide: by 0");
   ubigint power_of_2 = 1;
   ubigint divisor = that; // right operand, divisor
   ubigint quotient = 0;
   ubigint remainder = *this; // left operand, dividend




   while (divisor < remainder) {

      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }

     while (zero < power_of_2) {


      if (divisor < remainder or divisor == remainder) {

         remainder = remainder - divisor;
         quotient = quotient + power_of_2;


      }


      divisor.divide_by_2();
      power_of_2.divide_by_2();

   }


   return {quotient, remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {

    return divide (that).first;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return divide (that).second;
}

bool ubigint::operator == (const ubigint& that) const {
   if (this->ubig_value.size() != that.ubig_value.size()){
      return false;
   } else {
       int count = this->ubig_value.size() - 1;
       while (count >= 0){
           if (this->ubig_value.at(count) != that.ubig_value.at(count)){
               return false;
           }
           count --;

       }
       return true;

   }


}

bool ubigint::operator< (const ubigint& that) const {

    if (this->ubig_value.size() < that.ubig_value.size()){
           return true;
       } else if (this->ubig_value.size() > that.ubig_value.size()){
           return false;
       }
         else {

            if (((this->ubig_value.at(0) - '0') ==
                 (that.ubig_value.at(0) - '0')) and
                   this->ubig_value.size() == 1 ){
                return false;
            }


            if ((this->ubig_value.size() == 1) and
               ((this->ubig_value.at(0) - '0') >
                (that.ubig_value.at(0) - '0'))){
                return false;
            }

            int count = this->ubig_value.size() - 1;
            while (count >= 0){
                if ((this->ubig_value.at(count) - '0')
                        > (that.ubig_value.at(count) - '0')){
                    return false;
                }
                count --;

            }

            return true;

    }

}

ostream& operator<< (ostream& out, const ubigint& that) { 

      for (int count = that.ubig_value.size() - 1, j = 0;
           count >= 0 ;
           count --, j++){
         out << that.ubig_value.at(count);
         if(j==69){
             out << "/" << endl;
             j = 0;
         }
          }

      return out;
}

