.. _dev_docs_docs_testing:

Documentation Testing
=====================

This section describes the tests implemented for the *EasyNMEA* documentation:

1. ``easynmea-documentation-test``: An executable generated to check that all code snippets in the
   documentation compile.
   This way, whenever we make an API change, we will be forced to update the documentation to reflect it, and in that
   way we make sure that all the code in the documentation is up to date.
2. ``documentation.line_length``: RST files usually have a line length no longer than 120 characters.
   `Doc8` is used to check this for every RST file with argument `--max-line-length 120`.
3. ``documentation.spell_check``: A spelling check for the documentation.
   Sphinx builder *spelling* supports this, plus it also adds the possibility to have one or more custom dictionaries
   for words that the builder otherwise considers not correct.
4. ``documentation.link_check``: Checks that all documentation hyperlinks are valid.
   Sphinx supports this using `linkcheck` builder.

As defined in :ref:`dev_docs_testing_directories`, these tests are located in `<repo-root>/docs/test`.
Furthermore, it is possible to activate them with CMake option :class:`BUILD_DOCUMENTATION_TESTS` (see
:ref:`dev_docs_testing_build_tests`).
