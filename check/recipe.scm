(parameter decay 200 "the number of terms while the trail is hot")
(parameter depth 4096 "a depth of analysis")

(option primus-lisp-load
        posix
        memcheck-malloc
        limit-malloc
        taint-sources
        gather-spectre
        sensitive-sinks)

(option primus-lisp-channel-redirect
        <stdin>:$prefix/stdin
        <stdout>:$prefix/stdout)

(option report-progress)
(option log-dir log)

(option run)

(option primus-promiscuous-mode)
(option primus-greedy-scheduler)
(option primus-print-output incidents)
(option ddtbd-length $decay)
(option primus-limit-max-length $depth)

(option primus-lisp-add $prefix)
(option primus-print-obs
        spectre-path
        spectre-hypot-partial
        spectre-hypot-init
        pc-changed
        jumping
        call
        call-return
        machine-switch
        machine-fork
        undefined-input
        undefined-var
        undefined-ptr
        taint-attached
        lisp-message
        incident
        incident-location)

(option no-byteweight)
