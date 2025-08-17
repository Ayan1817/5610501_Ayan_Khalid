void decode_huff(node * root,string s)
{
    string ans = "";
    node* n = root;
    for(auto itr = s.begin(); itr != s.end();itr++){
        node* next;
        if(*itr == '0'){
            next = n -> left;
        }
        else{
            next = n -> right;
        }
        // Leaf node reached
        if(next -> left == NULL && next -> right == NULL){
            ans += next -> data;
            n = root;
        }
        else{
            n = next;
        }
    }
    cout << ans << endl;
}
