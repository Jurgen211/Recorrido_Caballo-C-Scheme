#lang racket

(define N 8)

(define archivo
  (open-output-file "RespuestaCaballo.txt" #:mode 'text #:exists 'replace)
  )

(define posIniciales
  (open-input-file "PosInicio.txt")
  )

(define vec
  (make-vector (* N N))
  )

(define tablero
  (let ([fila (for*/list ([x 2] [x* '(+1 -1)] [y* '(+1 -1)])
              (cons (* x* (+ 1 x)) (* y* (- 2 x))))])
    (for*/vector ([i N] [j N])
      (filter values (for/list ([d fila])
                       (let ([i (+ i (car d ))] [j (+ j (cdr d))])
                         (and (< -1 i N) (< -1 j N) (+ j (* N i))))
                       )
              )
      )
    )
  )

(define (solucion x y)
  (define pos (+ x (* N y)))
  (let repeticion ([L (list pos)] [val (vector-ref tablero pos)] [n (- (* N N) 1)])
    (if (zero? n) (reverse L)
        (for/or ([sgt (sort (map (λ(n) (cons n (remq* L (vector-ref tablero n)))) val) < #:key length #:cache-keys? #t)])
          (repeticion (cons (car sgt) L) (cdr sgt) (- n 1))
          )
        )
    )
  )

(define (agregar sol)
  (for ([num sol] [i (in-naturals 1)]) (vector-set! vec num i))
  )

(define (guardar sol)
  (agregar sol)
  (for ([i N])
    (for/list ([j (in-range i (* N N) N)])
      (write (vector-ref vec j) archivo) (newline archivo)
      )
    )
  )

(define (programa)
  (define x (read posIniciales) )
  (define y (read posIniciales) )
  (guardar (solucion x y))
  )

(programa)

(close-output-port archivo)

(close-input-port posIniciales)