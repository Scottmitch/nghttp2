.. program:: nghttpd

nghttpd(1)
==========

NAME
----
nghttpd - HTTP/2 experimental server

SYNOPSIS
--------
**nghttpd** [OPTION]... <PORT> <PRIVATE_KEY> <CERT>

**nghttpd** --no-tls [OPTION]... <PORT>

DESCRIPTION
-----------
HTTP/2 experimental server

.. option:: PORT

    Specify listening port number.

.. option:: PRIVATE_KEY

    Set  path  to  server's  private  key.   Required
    unless :option:`--no-tls` is specified.

.. option:: CERT

    Set  path  to   server's  certificate.   Required
    unless :option:`--no-tls` is specified.

OPTIONS
-------

.. option:: -D, --daemon

    
    Run in a background.  If  :option:`-D` is used, the current
    working directory  is changed to  '/'.  Therefore
    if  this  option  is  used,  :option:`-d`  option  must  be
    specified.

.. option:: -V, --verify-client

    
    The  server sends  a client  certificate request.
    If the  client did not return  a certificate, the
    handshake is terminated.   Currently, this option
    just requests  a client certificate and  does not
    verify it.

.. option:: -d, --htdocs=<PATH>

    
    Specify  document root.   If this  option is  not
    specified,  the  document  root  is  the  current
    working directory.

.. option:: -v, --verbose

    
    Print  debug   information  such   as  reception/
    transmission of frames and name/value pairs.

.. option:: --no-tls

    
    Disable SSL/TLS.

.. option:: -c, --header-table-size=<N>

    
    Specify decoder header table size.

.. option:: --color

    
    Force colored log output.

.. option:: -p, --push=<PATH>=<PUSH_PATH,...>

    
    Push  resources   <PUSH_PATH>s  when   <PATH>  is
    requested.  This option can be used repeatedly to
    specify multiple push configurations.  <PATH> and
    <PUSH_PATH>s are relative  to document root.  See
    :option:`--htdocs`    option.      Example:    -p/=/foo.png
    -p/doc=/bar.css

.. option:: -b, --padding=<N>

    
    Add  at most  <N>  bytes to  a  frame payload  as
    padding.  Specify 0 to disable padding.

.. option:: -n, --workers=<CORE>

    
    Set the number of worker threads.
    Default: 1

.. option:: -e, --error-gzip

    
    Make error response gzipped.

.. option:: --dh-param-file=<PATH>

    
    Path to  file that contains DH  parameters in PEM
    format.  Without  this option, DHE  cipher suites
    are not available.

.. option:: --early-response

    
    Start  sending response  when request  HEADERS is
    received,   rather  than   complete  request   is
    received.

.. option:: --version

    
    Display version information and exit.

.. option:: -h, --help

    
    Display this help and exit.

SEE ALSO
--------

nghttp(1), nghttpx(1), h2load(1)
