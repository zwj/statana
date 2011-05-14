USER=zwj;
case $1 in 
    release)
    git push $USER@zlabs.cn:/usr/local/git_store/statana.git master
    ;;
    push)
    git push $USER@zlabs.cn:/usr/local/git_store/statana_${USER}.git master
    ;;
    pull)
        echo "pull from who? (Yangtao, Zwj) ?"
        read W
        WHO=$USER
        case $W in 
            "y")
            WHO="yangtao"
            ;;
            "z")
            WHO="zwj"
            ;;
        esac
        echo "pull from $WHO"
        git pull $USER@zlabs.cn:/usr/local/git_store/statana_${WHO}.git master
    ;;
esac


