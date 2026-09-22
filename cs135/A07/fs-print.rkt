(module fs-print (lib "plt-pretty-big-text.ss" "lang")
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Data and type definitions
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; (Don't worry about the #f at the end of the define-struct lines.
  ;; That's a technical detail that doesn't matter in the teaching languages.)
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
  
  ;; A FileSystem (FS) is a:
  ;; * Dir
  
  ;; A FileDir is one of:
  ;; * a File
  ;; * a Dir
  
  ;; A FDList is one of:
  ;; * empty
  ;; * (cons FileDir FDList)
  
  (define-struct file (name size timestamp) #f)
  ;; A File is a (make-file String Nat Nat)
  
  (define-struct dir (name contents) #f)
  ;; A Dir is a (make-dir String FDList)

  (define sample-fs
    (make-dir "root"
              (list 
               (make-file "readme.txt" 187 1319502441)
               (make-dir "photos"
                         (list 
                          (make-file "zahra.jpg" 3669000 1319216638)
                          (make-file "timbit.jpg" 2866000 1319181806)
                          (make-file "chonky.jpg" 2709000 1319287654)
                          (make-dir "vacation"
                                    (list                    
                                     (make-file "beach1.jpg" 3297000 1319320493)
                                     (make-file "beach2.jpg" 2173000 1319449661)
                                     (make-file "beach3.jpg" 2747000 1319617966)))
                          (make-file "oreo.jpg" 3287000 1319542066)
                          (make-file "anna.jpg" 2294000 1319645092)
                          (make-file "beach1.jpg" 3297000 1319320493)))
               (make-dir "music"
                         (list 
                          (make-dir "rock"
                                    (list 
                                     (make-file "eagles-hotel-california.mp3" 10184000 1319658262)
                                     (make-file "bee-gees-stayin-alive.mp3" 9693000 1319660517)))
                          (make-dir "dance"
                                    (list 
                                     (make-file "lady-gaga-bad-romance.mp3" 9376000 1319612418)
                                     (make-file "beyonce-single-ladies.mp3" 17669000 1319207025)))))
               (make-dir "schoolwork" empty)
               (make-dir "notes"
                         (list 
                          (make-file "shopping.txt" 573 1319655039)
                          (make-file "todo.txt" 301 1319679565))))))
  
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; Code for Pretty Printing a FileSystem with fs-print
  ;; (minimal design recipe)
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
  ;; lofd-print: (listof (union File Dir)) (listof boolean) -> String
  ;; Purpose: Recurse through a directory (lofd), dispatching each entry
  ;;          to fd-print.  lob 'accumulates' information about whether
  ;;          or not it's the last entry in the dir
  (define (lofd-print lofd lob)
    (cond
      [(empty? lofd) ""]
      [else (string-append 
             (fd-print (first lofd)
                       (append lob (list (empty? (rest lofd)))))
             (lofd-print (rest lofd) lob))]))
  
  ;; fd-print (union File Dir) (listof Boolean) -> String
  ;; Purpose: Print a File or a Dir (and then recurse if it's a Dir)
  ;;          The lob ride-along is used for indentation
  (define (fd-print fd lob)
    (cond 
      [(file? fd) (string-append (fd-print-indent lob)
                                 (string #\u2500 #\space )
                                 (file-name fd) "\n")]
      [else (string-append (fd-print-indent lob)
                           (string #\u25bc #\space ) 
                           (dir-name fd) "\n"
                           (lofd-print (dir-contents fd) lob))]))
  
  ;; fd-print-indent (listof Boolean) -> String
  ;; Purpose: Print the indentation / lines of the filesystem view
  ;;          each Boolean corresponds to a "level of indentation"
  ;;          and is true if we are on the last entry of that level
  (define (fd-print-indent lob)
    (cond [(empty? lob) ""]
          [else 
           (string-append 
            (cond
              ;; lowest level and last entry, so use "L"
              [(and (first lob) (empty? (rest lob))) (string #\u2514 #\u2500)]
              ;; lowest level so use "T"
              [(empty? (rest lob)) (string #\u251c #\u2500)]
              ;; higher level, but last entry, so just blank
              [(first lob) (string #\space #\space)]
              ;; higher level, but not last entry so use "|"
              [else (string #\u2502 #\space)])
            ;; recurse to next (lower) level
            (fd-print-indent (rest lob)))]))
  
  ;; fs-print FileSystem -> void
  ;; Purpose: Print a pretty FileSystem, a wrapper for fd-print
  (define (fs-print fs)
    (display (fd-print fs empty)))
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  
  ;; this "provides" the following so they are visible when you "require" them 
  
  (provide
   
   make-file
   file?
   file-name
   file-size 
   file-timestamp
   
   make-dir
   dir?
   dir-name
   dir-contents
   
   fs-print
   
   sample-fs))
