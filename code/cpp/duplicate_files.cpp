/*
Dropbox

Duplicate Files

https://leetcode.com/problems/find-duplicate-file-in-system/

Given a file system, return a list of collections of duplicate files. 

Ask about:
Symbolic link, same file/dir with diff name, cannot detect cycle by visited...cycle?
-use absolute path/ skip symbolic link (if we search the whole file system)

What about invalid or malformed files e.g. permission or cannot read
-compare file by hashing (MD5, SHA)

If dir depth is large: DFS might stack overflow, use BFS; the variable to store pathname might overflow.
-Most memory consuming: MD5, read in files etc

What about race conditions, like if someone is writing the file while you are reading etc

What if the process takes a long time? 
-If error / hanging up in between: checkpoints, save states from time to time

1. Imagine you are given a real file system, how will you search files? DFS or BFS ?
In general, BFS will use more memory then DFS. However BFS can take advantage of the locality of files in inside directories, and therefore will probably be faster

2. If the file content is very large (GB level), how will you modify your solution?
In a real life solution we will not hash the entire file content, since it's not practical. Instead we will first map all the files according to size. 
Files with different sizes are guaranteed to be different. We will than hash a small part of the files with equal sizes (using MD5 for example). 
Only if the md5 is the same, we will compare the files byte by byte

3. If you can only read the file by 1kb each time, how will you modify your solution?
This won't change the solution. We can create the hash from the 1kb chunks, and then read the entire file if a full byte by byte comparison is required.

What is the time complexity of your modified solution? What is the most time consuming part and memory consuming part of it? How to optimize?
Time complexity is O(n^2 * k) since in worse case we might need to compare every file to all others. k is the file size

How to make sure the duplicated files you find are not false positive?
We will use several filters to compare: File size, Hash and byte by byte comparisons.

Also, to answer this What is the most time consuming part and memory consuming part of it? How to optimize? part:
Comparing the file (by size, by hash and eventually byte by byte) is the most time consuming part.
Generating hash for every file will be the most memory consuming part.
We follow the above procedure will optimize it, since we compare files by size first, only when sizes differ, 
we'll generate and compare hashes, and only when hashes are the same, we'll compare byte by byte.
Also, using better hashing algorithm will also reduce memory/time.
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cassert>

// if we want to be more efficent we can only get hashes of files that are the same size/length but that takes up more space

std::vector<std::vector<std::string>> findDuplicate(const std::vector<std::string>& paths) {
    // use hashing- -> for hashing use map
    std::unordered_map<std::string, std::vector<std::string>> files;
    std::vector<std::vector<std::string>> result;

    for (auto path : paths) {
        // whenever you want to split the string use stringstream:
        std::stringstream ss(path);
        std::string root;
        std::string s;
        getline(ss, root, ' '); // split root from file so that we can comp strings and for that just look for the firt space
        while(getline(ss, s, ' ')) { // now split the files
            std::string fileName = root + '/' + s.substr(0, s.find('('));
            std::string fileContent = s.substr(s.find('(') + 1, s.find(')') - s.find('(') - 1);
            files[fileContent].push_back(fileName);
        }
    }

    for (const auto& file : files) {
        if (file.second.size() > 1) { // check if there are more thatn one files in a vector to determine dupe
            result.push_back(file.second);
        }
    }
    return result;
}

std::unordered_map<int, std::vector<std::string>> files_with_same_size(const std::vector<std::string>& paths) {
    std::unordered_map<int, std::vector<std::string>> same_size_map;
     for (auto path : paths) {
        // whenever you want to split the string use stringstream:
        std::stringstream ss(path);
        std::string root;
        std::string s;
        getline(ss, root, ' '); // split root from file so that we can comp strings and for that just look for the firt space
        while(getline(ss, s, ' ')) { // now split the files
            std::string fileContent = s.substr(s.find('(') + 1, s.find(')') - s.find('(') - 1);
            same_size_map[fileContent.size()].push_back(root + '/' + s);
        }
    }

    for (const auto&[key, values] : same_size_map) {
        std::cout<< key << ": ";
        for (auto const& val : values) {
            std::cout<< val << " ";
        }
        std::cout<< std::endl;

    }
    return same_size_map;
}

std::vector<std::vector<std::string>> findDuplicatev2(const std::vector<std::string>& paths) {
    // use hashing- -> for hashing use map
    std::unordered_map<int, std::vector<std::string>> same_size_map = files_with_same_size(paths);
    std::unordered_map<std::string, std::vector<std::string>> files;
    std::vector<std::vector<std::string>> result;

    for (const auto&[file_size, file_paths] : same_size_map) {
        if (file_paths.size() > 1) {
            for (const auto& path : file_paths) {
                    std::string fileContent = path.substr(path.find('(') + 1, path.find(')') - path.find('(') - 1);
                    files[fileContent].push_back(path.substr(0, path.find('(')));
                }
            }
        }

    for (const auto& file : files) {
        if (file.second.size() > 1) { // check if there are more thatn one files in a vector to determine dupe
            result.push_back(file.second);
        }
    }
    return result;
}

int main() {
    std::vector<std::vector<std::string>> ans = findDuplicatev2({"root/a 1.txt(abcde) 2.txt(efgh)","root/c 3.txt(abcdef)","root/c/d 4.txt(efxh)","root 4.txt(efgh)"});
    for (const auto& vec : ans) {
        for (const auto& e : vec) {
            std::cout<< e << " ";
        }
        std::cout<<std::endl;
    }
    assert(ans[0][0] == "root/a/2.txt");
    assert(ans[0][1] == "root/4.txt");
    return 0;
}

#if 0
class DuplicateFiles:
    mb = 1024 * 1024

    def __init__(self, root):
        self.result = []
        self.size_to_files = {}
        self.root = root

    def get_hash(self, file):
        //Returns the SHA 256 hash of the file
        output_hash = hashlib.sha256()
        with open(file, "rb") as file_obj:
            mb_chunk = file_obj.read(mb)
            if mb_chunk is not None:
                output_hash.update(mb_chunk)
            else:
                break
        return output_hash.hexdigest()
    
    def add_file(self, file):
        if file.file_size in self.size_to_files:
            self.size_to_files[file.file_size].append(file)
        else:
            self.size_to_files[file.file_size] = [file]

    def group_files_by_size(self):
        //Populates self.size_to_files with the sizes and the files with those sizes
        queue = collections.deque()
        queue.appendleft(self.root)
        seen = set()
        while queue:
            current_folder = queue.pop()
            seen.add(current_folder)
            for content in current_folder.iter_dir(): //iterdir is the contents of the file, both files and folders
                if content.is_directory() and content not in seen:
                    queue.appendleft(content)
                    seen.add(content)
                elif content.is_file():
                    self.add_file(content)
                else:
                    //Ask the interviewer how to handle symlinks or special cases
                    pass

    def process_files(self):
        //Returns list of collections of duplicate files
        //First, group the files by size
        self.group_files_by_size()

        //Now you have the files grouped by size
        //For the sizes with more than one file, you need to deduplicate
        result = []
        for size, files in self.size_to_files.items():
            if len(files) > 1:
                hash_groups = {} //Map <hash: str, files with that hash: List[File]>
                for file in files:
                    file_hash = self.get_hash(file)
                    if file_hash in hash_groups:
                        hash_groups[file_hash].append(file)
                    else:
                        hash_groups[file_hash] = [file]
                for list_of_files in hash_groups.values():
                    if len(list_of_files) > 1:
                        result.append(list_of_files)
        return result

#endif

//Then call duplicate_files = DuplicateFiles(root) and return duplicate_files.process()
