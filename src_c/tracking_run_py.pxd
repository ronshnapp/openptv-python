
cdef extern from "parameters.h":
    ctypedef struct sequence_par:
        pass

cdef extern from "tracking_run.h":
    ctypedef struct tracking_run:
        sequence_par *seq_par
    cdef void tr_free(tracking_run *tr)

cdef class TrackingRun:
    cdef tracking_run *tr
