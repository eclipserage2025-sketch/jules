from setuptools import setup, Extension

scrypt_module = Extension('ltc_scrypt',
                          sources=['scryptmodule.c'])

setup(name='ltc_scrypt',
      version='1.0',
      description='Litecoin Scrypt hashing extension',
      ext_modules=[scrypt_module])
