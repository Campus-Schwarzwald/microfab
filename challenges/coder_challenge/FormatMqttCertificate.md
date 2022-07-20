# Formatting certificates and keys for the MQTT client

To save formatting work, the certificates and the key must be formatted correctly in the MQTT client. 
This example shows how the formatting can be done via sed

## Before

    -----BEGIN CERTIFICATE-----
    MIIC0zCCAnmgAwIBAgIUf0goDJhTaEMrnccLFUNZLFR/I98wCgYIKoZIzj0EAwQw
    HjEcMBoGA1UEAwwTS0YtQ1MtSEFDS1RIRUZBQi1DQTAeFw0yMjA3MTgxNDMyMzha
    Fw0yMzA3MDMxNDMyMzdaMGkxEDAOBgNVBA0MB0dyb3VwXzExFzAVBgNVBAMMDjE5
    Mi4xNjguODguMTAyMRMwEQYDVQQLDApIYWNrVGhlRmFiMRowGAYDVQQKDBFDYW1w
    dXNTY2h3YXJ6d2FsZDELMAkGA1UEBhMCREUwggEiMA0GCSqGSIb3DQEBAQUAA4IB
    DwAwggEKAoIBAQCnn3H1mRY/wIbtcxlmr0n5A1MUAAjuaSSrx12TCIqb2R76ugCA
    LPwZ1p2URaJVjWvDS6i7qo4MRw0KkTcOkCMCP8cLz8s99QXWbGDx3UG1KxTZnkDW
    b7OxemBLV9vwPXfiIvf7PRQbRYLVt3CyyKxMQbPDzBKN+WY0WXMhrNZctWLaPr32
    gBaKzpsfizLlMNl087iV+PV9hahHqk/bBbZyj2icmwq37NBn/B/5B/ghyfLlJBd7
    luA5jzB3qYRubjN8yGGIWeiixnputAs44ZxJMSBfL04HJyFuSjtanr2PVSoFw08K
    NVuf7WIDlrGLRwpowVGnPvqEQipVG+1/o7/TAgMBAAGjfzB9MAwGA1UdEwEB/wQC
    MAAwHwYDVR0jBBgwFoAUYEM8tBJPLlsrjrFdb9ijuHxcELQwHQYDVR0lBBYwFAYI
    KwYBBQUHAwIGCCsGAQUFBwMBMB0GA1UdDgQWBBQQorvJpxAT7pWkjuNVAlavjQJI
    yTAOBgNVHQ8BAf8EBAMCBaAwCgYIKoZIzj0EAwQDSAAwRQIgEyvix3WLz4igPZSU
    I82D11EXbQ3s+24/a2HsM93fQDoCIQD/O72EC5YmTsf4AcSGpciSrBiiuzASDaMj
    BYK5VvUtNg==
    -----END CERTIFICATE-----

## After

    "-----BEGIN CERTIFICATE-----\n" \
    "MIIC0zCCAnmgAwIBAgIUf0goDJhTaEMrnccLFUNZLFR/I98wCgYIKoZIzj0EAwQw\n" \
    "HjEcMBoGA1UEAwwTS0YtQ1MtSEFDS1RIRUZBQi1DQTAeFw0yMjA3MTgxNDMyMzha\n" \
    "Fw0yMzA3MDMxNDMyMzdaMGkxEDAOBgNVBA0MB0dyb3VwXzExFzAVBgNVBAMMDjE5\n" \
    "Mi4xNjguODguMTAyMRMwEQYDVQQLDApIYWNrVGhlRmFiMRowGAYDVQQKDBFDYW1w\n" \
    "dXNTY2h3YXJ6d2FsZDELMAkGA1UEBhMCREUwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
    "DwAwggEKAoIBAQCnn3H1mRY/wIbtcxlmr0n5A1MUAAjuaSSrx12TCIqb2R76ugCA\n" \
    "LPwZ1p2URaJVjWvDS6i7qo4MRw0KkTcOkCMCP8cLz8s99QXWbGDx3UG1KxTZnkDW\n" \
    "b7OxemBLV9vwPXfiIvf7PRQbRYLVt3CyyKxMQbPDzBKN+WY0WXMhrNZctWLaPr32\n" \
    "gBaKzpsfizLlMNl087iV+PV9hahHqk/bBbZyj2icmwq37NBn/B/5B/ghyfLlJBd7\n" \
    "luA5jzB3qYRubjN8yGGIWeiixnputAs44ZxJMSBfL04HJyFuSjtanr2PVSoFw08K\n" \
    "NVuf7WIDlrGLRwpowVGnPvqEQipVG+1/o7/TAgMBAAGjfzB9MAwGA1UdEwEB/wQC\n" \
    "MAAwHwYDVR0jBBgwFoAUYEM8tBJPLlsrjrFdb9ijuHxcELQwHQYDVR0lBBYwFAYI\n" \
    "KwYBBQUHAwIGCCsGAQUFBwMBMB0GA1UdDgQWBBQQorvJpxAT7pWkjuNVAlavjQJI\n" \
    "yTAOBgNVHQ8BAf8EBAMCBaAwCgYIKoZIzj0EAwQDSAAwRQIgEyvix3WLz4igPZSU\n" \
    "I82D11EXbQ3s+24/a2HsM93fQDoCIQD/O72EC5YmTsf4AcSGpciSrBiiuzASDaMj\n" \
    "BYK5VvUtNg==\n" \
    "-----END CERTIFICATE-----\n" \

```bash
openssl x509 -in YourCertificate.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > YourCertificatePre.txt
sed 's/^/"/; s/$/\\n" \\/' YourCertificatePre.txt > YourCertificate.txt
```
