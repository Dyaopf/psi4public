/*
 *@BEGIN LICENSE
 *
 * PSI4: an ab initio quantum chemistry software package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *@END LICENSE
 */

#ifndef FRAGOPTIONS_H_
#define FRAGOPTIONS_H_


#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include "exception.h"

namespace psi{
class Molecule;
typedef boost::shared_ptr<Molecule> SharedMol;
namespace LibFrag{
enum FragMethods {USER_DEFINED,BOND_BASED,DISTANCE_BASED};
enum EmbedMethods {NO_EMBED,POINT_CHARGE,ITR_POINT_CHARGE,DENSITY,
                   ITR_DENSITY};
enum CapMethods {NO_CAPS,H_REPLACE,H_SHIFTED};
enum BSSEMethods {NO_BSSE,FULL,MBCPN,VMFCN};

class GMBE;
class Fragmenter;
class BSSEer;
class Capper;
class Embedder;

template<typename T,typename T2>
T2 MapDeRef(const std::map<T,T2>& Map,const T& Key,const std::string& Error){
   T2 returnvalue;
   if(Map.count(Key)==1)
      returnvalue=(const_cast<std::map<T,T2>* >(&Map))->operator[](Key);
   else
      throw PSIEXCEPTION(Error.c_str());
   return returnvalue;
}


template<typename T,typename T2>
class CoreOptions{
   private:
      void Copy(const CoreOptions<T,T2>& other){
         this->DaMethod_=other.DaMethod_;
         this->Methods_=other.Methods_;
         this->Converter_=other.Converter_;
      }
   protected:
      ///This is the method
      T DaMethod_;

      ///Map that prints out the official name of our methods
      std::map<T,std::string> Methods_;

      ///Map that takes officially recognized aliases and returns the method
      std::map<std::string,T> Converter_;

   public:
      virtual boost::shared_ptr<T2> MakeFactory(SharedMol& AMol)const=0;
      std::string MethodName()const{
         return MapDeRef(Methods_,DaMethod_,"Not Possible");
      }
      T Method()const{return DaMethod_;}
      void SetMethod(const T Method){DaMethod_=Method;}
      void SetMethod(const std::string& Method){
         std::string Error="Desired method:" + Method+ " not found";
         DaMethod_=MapDeRef(Converter_,Method,Error);
      }
      CoreOptions<T,T2>(const CoreOptions<T,T2>& other){this->Copy(other);}
      CoreOptions<T,T2>(T Method):DaMethod_(Method){}
      virtual ~CoreOptions(){}
};

class FragOptions: public CoreOptions<FragMethods,Fragmenter>{
   private:
      void SetDefaults();
   public:
      boost::shared_ptr<Fragmenter> MakeFactory(SharedMol& AMol)const;
      FragOptions(FragMethods Method=USER_DEFINED):
         CoreOptions<FragMethods,Fragmenter>(Method){
         SetDefaults();
      }
};

class EmbedOptions: public CoreOptions<EmbedMethods,Embedder>{
   private:
      void SetDefaults();
   public:
      boost::shared_ptr<Embedder> MakeFactory(SharedMol& AMol)const;
      EmbedOptions(EmbedMethods Method=NO_EMBED):
         CoreOptions<EmbedMethods,Embedder>(Method){
         SetDefaults();
      }
};

class CapOptions: public CoreOptions<CapMethods,Capper>{
   private:
      void SetDefaults();
   public:
      boost::shared_ptr<Capper> MakeFactory(SharedMol& AMol)const;
      CapOptions(CapMethods Method=NO_CAPS):
         CoreOptions<CapMethods,Capper>(Method){
         SetDefaults();
      }
};

class BSSEOptions: public CoreOptions<BSSEMethods,BSSEer>{
   private:
      void SetDefaults();
   public:
      boost::shared_ptr<BSSEer> MakeFactory(SharedMol& AMol)const;
      BSSEOptions(BSSEMethods Method=NO_BSSE):
         CoreOptions<BSSEMethods,BSSEer>(Method){
         SetDefaults();
      }
};

class LibFragOptions{
	private:
        FragOptions FOptions_;
        CapOptions COptions_;
        BSSEOptions BOptions_;
        EmbedOptions EOptions_;

	public:
		const FragOptions& FOptions(){return FOptions_;}
		const CapOptions& COptions(){return COptions_;}
		const BSSEOptions& BOptions(){return BOptions_;}
		const EmbedOptions& EOptions(){return EOptions_;}

		///The MBE truncation order
		int MBEOrder_;

		///Are we exploiting symmetry
		bool DoSymm_;

        ///Nice, pretty printing of all the desired options
        void PrintOptions();

        LibFragOptions(const int MBEOrder,const std::string& FMethod,
              const std::string& EMethod,const std::string& CMethod,
              const std::string& BMethod,bool DoSymm=false):MBEOrder_(MBEOrder),
                    DoSymm_(DoSymm){
           FOptions_.SetMethod(FMethod);
           EOptions_.SetMethod(EMethod);
           BOptions_.SetMethod(BMethod);
           COptions_.SetMethod(CMethod);
        }
		~LibFragOptions(){}
};

}}// End namespaces


#endif /* FRAGOPTIONS_H_ */
