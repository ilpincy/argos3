_argos3() {
    # declare some local variables
    local cur prev opts
    # erase the content of COMPREPLY
    COMPREPLY=()
    # current token
    cur="${COMP_WORDS[COMP_CWORD]}"
    # previous token
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    # option list
    opts="-h --help -v --version -c --config-file -q --query -n --no-color -l --log-file -e --logerr-file"
    # Complete option arguments
    case "${prev}" in
        -h|--help|-v|--version|-n|--no-color)
            return 0
            ;;
        -c|--config-file)
            _filedir argos
            return 0
            ;;
        -q|--query)
            local plugintypes=all actuators sensors physics_engines media visualizations entities
            local plugins=$(argos3 -n -q all | grep -v '^\[INFO\]' | grep '\[ ' | tr -d "[]()" | cut -c 5- | cut -d\  -f1 | sort | xargs)
            COMPREPLY=( $(compgen -W "${plugintypes} ${plugins}" -- ${cur}) )
            return 0
            ;;
        -l|--log-file|-e|--logerr-file)
            COMPREPLY=( $(compgen -f ${cur}) )
            return 0
            ;;
        *)
            ;;
    esac
    # Complete options
    COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
}

complete -F _argos3 argos3
