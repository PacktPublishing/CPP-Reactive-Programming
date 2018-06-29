#include <rxcpp/rx.hpp>
#include <memory>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <windows.h>
#include <functional>
#include <thread>
#include <future>
using namespace std;
////////////////////////////////////
//-------------- Forward Declarations
//-------------- Model Folder/File
class FileNode;
class DirectoryNode;
////////////////////////////////
//------------- The Visitor Interface
class IFileFolderVisitor;

/////////////////////////////////
//------ a Type to store FileInformation
struct FileInformation
{
   string name;
   long size;
   FileInformation( string pname,long psize )
   { name = pname;size = psize; }
};
//////////////////////////////
//-------------- Base class for File/Folder data structure
class EntryNode{
    protected:
      string  name;
      int isdir;
      long size;
    public:
      virtual bool Isdir() = 0;
      virtual long getSize() = 0;
      virtual void Accept(IFileFolderVisitor& ivis)=0;
      virtual ~EntryNode() {}
};

////////////////////////////////
//
//-------------The Visitor Interface
//
class IFileFolderVisitor
{
   public:
    virtual void Visit(FileNode& fn )=0;
    virtual void Visit(DirectoryNode& dn )=0;
};
//////////////////////////////////////
//
// The Node which represents Files
//
//
class FileNode : public EntryNode {
    
   public: 
   FileNode(string pname, long psize) { 
	isdir = 0; name = pname; size = psize;
   }
   ~FileNode() {cout << "....Destructor FileNode ...." << name << endl; }
   virtual bool  Isdir() { return isdir == 1; }
   string getname() { return name; }
   virtual long getSize() {return size; }
   virtual void Accept( IFileFolderVisitor& ivis ){
       ivis.Visit(*this);
    }

};

///////////////////////////////////////
// Node which represents Directory
//
//

class DirectoryNode : public EntryNode {
  list<unique_ptr<EntryNode>> files;  
public:
  DirectoryNode(string pname) 
  { files.clear(); isdir = 1; name = pname;}
  ~DirectoryNode() {
     cout << "Destructor DirectoryNode...." << name << endl;
     files.clear();
  }
  list<unique_ptr<EntryNode>>& GetAllFiles() {
       return files;
  }
  bool AddFile( string pname , long size) {
       files.push_back(unique_ptr<EntryNode>
                     (new FileNode(pname,size)));
       return true;
  }
  bool AddDirectory( DirectoryNode *dn ) {
	files.push_back(unique_ptr<EntryNode>(dn));
        return true;
  }
  bool Isdir() { return isdir == 1; }
  string  getname() { return name; }
  void   setname(string pname) { name = pname; }
  long getSize() {return size; }
  void Accept( IFileFolderVisitor& ivis ){
       ivis.Visit(*this);
    }

};



/////////////////////////////////////////////
//--------------- Directory Helper
//--------------- Has to be written for Each OS
class DirHelper {

   public:
    static 
    DirectoryNode 
    *SearchDirectory(const std::string& refcstrRootDirectory)
    {
       std::string     strFilePath;             // Filepath
       HANDLE          hFile;                   // Handle to file
       WIN32_FIND_DATA FileInformation;         // File information
       string strPattern = refcstrRootDirectory + "\\*.*";
      
       hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);

       if ( hFile == INVALID_HANDLE_VALUE ) { return 0; }
        DirectoryNode *DirNode = 
                  new DirectoryNode(refcstrRootDirectory);
        do {
           if(FileInformation.cFileName[0] == '.')
                   continue;

                strFilePath.erase();
                strFilePath = refcstrRootDirectory +
                             "\\" + FileInformation.cFileName;
                if(FileInformation.dwFileAttributes & 
                              FILE_ATTRIBUTE_DIRECTORY){
                 // Search subdirectory
                 DirectoryNode *temp = 
                     (DirectoryNode *) SearchDirectory(strFilePath);
                 if(temp) 
                     DirNode->AddDirectory(temp);
                }
                else
                  DirNode->AddFile(strFilePath,1024);
          
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);
    return DirNode;
  }


};

/////////////////////////////////////
//
//----- A Visitor Interface which prints
//----- The contents of a Folder
class PrintFolderVisitor : public IFileFolderVisitor
{
  public:
    void Visit(FileNode& fn ) {
      cout << fn.getname() << endl;
    }
    void Visit(DirectoryNode& dn ) {
      cout << "In a directory " << dn.getname() << endl;
      list<unique_ptr<EntryNode>>& ls = dn.GetAllFiles();
      for ( auto& itr : ls ) 
        itr.get()->Accept(*this);
    }
 
};


//////////////////////////////////////
//
// Flatten the File/Folders into a linear list
//
//
class FlattenVisitor : public IFileFolderVisitor
{
    list <FileInformation> files;
    string CurrDir;
 public:
    FlattenVisitor() { CurrDir = "";}
    ~FlattenVisitor() { files.clear();}
    list<FileInformation> GetAllFiles() {
         return files;
    }
    void Visit(FileNode& fn ) {
       files.push_back( FileInformation(
          CurrDir +"\\" + fn.getname(),fn.getSize()));
    }
    void Visit(DirectoryNode& dn ) {
        CurrDir = dn.getname();
        files.push_back( FileInformation(
            CurrDir, 0 ));
        list<unique_ptr<EntryNode>>& ls = dn.GetAllFiles();
        for ( auto& itr : ls ) 
            itr.get()->Accept(*this);
    }
};



list<FileInformation> GetAllFiles(string dirname )
{

   // Search 'c:' for '.avi' files including subdirectories
   DirectoryNode *dirs =DirHelper::SearchDirectory(dirname);

   if ( dirs == 0 ) {
    list<FileInformation> ret_val;
    return ret_val;
   }

   FlattenVisitor *fs = new FlattenVisitor();
   dirs->Accept(*fs);
   list<FileInformation> ret_val = fs->GetAllFiles();
   delete fs;
   delete dirs;
   return ret_val;

}


void TestVisitor( string directory )
{


  // Search 'c:' for '.avi' files including subdirectories
  DirectoryNode *dirs = DirHelper::SearchDirectory(directory);

  if ( dirs == 0 ) {
    cout << "Notrhing to dispaly" << endl;
    return;
  }

  PrintFolderVisitor *fs = new PrintFolderVisitor ();
  dirs->Accept(*fs);

  delete fs;
  delete dirs;

   

}

//////////////////////////////
// A Toy implementation of Active 
// Object Pattern...
template <class T>
struct ActiveObject {
    rxcpp::subjects::subject<T> subj;
    // fire-and-forget
    void FireNForget(T & item)
    {
           subj.get_subscriber().on_next(item);
    }
 
    rxcpp::observable<T> GetObservable() 
    { return subj.get_observable(); }

    ActiveObject(){} 
    ~ActiveObject() {}
};
///////////////////////
// The class uses a FireNForget mechanism to 
// push data to the Data/Event sink
//
class DirectoryEmitter
{
      string rootdir;
      ActiveObject<FileInformation> act;
  public:
      DirectoryEmitter(string s )   {
         rootdir = s;
         //----- Subscribe 
         act.GetObservable().subscribe([] ( FileInformation item ) {
            cout << item.name << ":" << item.size << endl;
         });
      }
    
      bool Trigger() {
           std::packaged_task<int()> task([&]() {
                  EmitDirEntry(); return 1;
           });
           std::future<int> result = task.get_future();
           task();
           //------------ Uncomment the below line 
           //------------ to return immediately
           double dresult = result.get();
           return true;

      }
      //----- Iterate over the list of files 
      bool EmitDirEntry() {
           list<FileInformation> rs = GetAllFiles(rootdir);
           for( auto& a : rs )
              act.FireNForget(a);
  	   return false;
      }
};

int main(int argc, char *argv[]) {

  //TestVisitor("D:\\Java");
	
  //list<FileInformation> rs = GetAllFiles("D:\\JAVA");

  //for( auto& as : rs )
  //  cout << as.name << endl; 

  DirectoryEmitter emitter("D:\\JAVA");
  emitter.Trigger();

  return 0;

}

