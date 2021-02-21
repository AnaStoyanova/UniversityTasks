#include<iostream>
#include<vector>
#include<string>

std::vector<int> sort_cyclic_shifts(std::string s) {

    int n = s.size();
    const int alphabet = 256;
    std::vector<int> p(n), c(n), cnt(std::max(alphabet, n), 0);

    for (int i = 0; i < n; i++) {
        cnt[s[i]]++;
    }
    for (int i = 1; i < alphabet; i++) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = 0; i < n; i++) {
        p[--cnt[s[i]]] = i;
    }

    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i - 1]]) {
            classes++;
        }
        c[p[i]] = classes - 1;
    }

    std::vector<int> pn(n), cn(n);
    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0) {
                pn[i] += n;
            }
        }

        fill(cnt.begin(), cnt.begin() + classes, 0);

        for (int i = 0; i < n; i++) {
            cnt[c[pn[i]]]++;
        }
        for (int i = 1; i < classes; i++) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = n - 1; i >= 0; i--) {
            p[--cnt[c[pn[i]]]] = pn[i];
        }
           

        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            std::pair<int, int> cur = { c[p[i]], c[(p[i] + (1 << h)) % n] };
            std::pair<int, int> prev = { c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n] };
            if (cur != prev) {
                ++classes;
            }
           
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
    }

    return p;
}

std::vector<int> suffix_array_construction(std::string s) {
    s += "$";
    std::vector<int> sorted_shifts = sort_cyclic_shifts(s);

    return sorted_shifts;
}

std::vector<int> search(const char* searchWord, const char* txt, std::vector<int> suffArr)
{
    std::vector<int> occurances;

    int lengthSearchWord = strlen(searchWord);

    int l = 0, r = strlen(txt) - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
       
        int res = strncmp(searchWord, txt + suffArr[mid], lengthSearchWord);

        if (res == 0) {
            occurances.push_back(suffArr[mid]);

            int remInd = mid;
         
            while (mid - 1 >= 0 && strncmp(searchWord, txt + suffArr[--mid], lengthSearchWord) == 0) {
                occurances.push_back(suffArr[mid]);
            }

            mid = remInd;

            while (mid + 1 <= suffArr.size() - 1 && strncmp(searchWord, txt + suffArr[++mid], lengthSearchWord) == 0) {
                occurances.push_back(suffArr[mid]);
            }

            return occurances;
        }

        if (res < 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
}


int main() {
    std::string txt = "This is some example text.";

    std::vector<int> suff = suffix_array_construction(txt);

    int q; // number of requests
    std::cout << "Enter number of requests: ";
    std::cin >> q;
    std::cin.ignore();


    for (int i = 0; i < q; i++) {

        std::cout << "\nEnter the word you want to search: ";
        std::string searchWord;
        getline(std::cin, searchWord);

        std::vector<int> result = search(searchWord.c_str(), txt.c_str(), suff);
        int count = result.size();
        std::cout << "Count: " << count << '\n';

        if (count == 0) {
            continue;
        }

        std::cout << "Do you want to see the indexes of appearance? \n[1]Yes [0]No \n";
        bool option;
        std::cin >> option;
        std::cin.ignore();

        if (option) {
            std::cout << "Indexes: ";
            for (int i = 0; i < result.size(); i++) {
                std::cout << result[i] << " ";
            }
        }
        
        std::cout << '\n';

    }

    return 0;
}