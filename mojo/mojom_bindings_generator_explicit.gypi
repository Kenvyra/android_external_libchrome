# Copyright 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'includes': [
    'mojom_bindings_generator_variables.gypi',
  ],
  'variables': {
    'variables': {
      'mojom_variant%': 'none',
    },
    'mojom_variant%': '<(mojom_variant)',
    'mojom_base_output_dir':
        '<!(python <(DEPTH)/build/inverse_depth.py <(DEPTH))',
    'mojom_generated_outputs': [
      '<!@(python <(DEPTH)/mojo/public/tools/bindings/mojom_list_outputs.py --basedir <(mojom_base_output_dir) --variant <(mojom_variant) <@(mojom_files))',
    ],
    'mojom_include_path%': '<(DEPTH)',
    'mojom_extra_generator_args%': [],
    'require_interface_bindings%': 1,
    'conditions': [
      ['mojom_variant=="none"', {
        'mojom_output_languages%': 'c++,javascript,java',
      }, {
        'mojom_output_languages%': 'c++',
      }],
    ],
  },
  # Given mojom files as inputs, generate sources.  These sources will be
  # exported to another target (via dependent_settings) to be compiled.  This
  # keeps code generation separate from compilation, allowing the same sources
  # to be compiled with multiple toolchains - target, NaCl, etc.
  'actions': [
    {
      'variables': {
        'java_out_dir': '<(PRODUCT_DIR)/java_mojo/<(_target_name)/src',
        'stamp_filename': '<(PRODUCT_DIR)/java_mojo/<(_target_name)/<(_target_name).stamp',
      },
      'action_name': '<(_target_name)_mojom_bindings_stamp',
      # The java output directory is deleted to ensure that the java library
      # doesn't try to compile stale files.
      'action': [
        'python', '<(DEPTH)/build/rmdir_and_stamp.py',
        '<(java_out_dir)',
        '<(stamp_filename)',
      ],
      'inputs': [
        '<@(mojom_files)',
        '<(SHARED_INTERMEDIATE_DIR)/mojo/public/tools/bindings/cpp_templates.zip',
        '<(SHARED_INTERMEDIATE_DIR)/mojo/public/tools/bindings/java_templates.zip',
        '<(SHARED_INTERMEDIATE_DIR)/mojo/public/tools/bindings/js_templates.zip',
      ],
      'outputs': [ '<(stamp_filename)' ],
    },
    {
      'action_name': '<(_target_name)_mojom_bindings_generator',
      'variables': {
        'java_out_dir': '<(PRODUCT_DIR)/java_mojo/<(_target_name)/src',
        'stamp_filename': '<(PRODUCT_DIR)/java_mojo/<(_target_name)/<(_target_name).stamp',
        'mojom_import_args%': [
         '-I<(DEPTH)',
         '-I<(DEPTH)/mojo/services',
         '-I<(mojom_include_path)',
        ],
      },
      'inputs': [
        '<@(mojom_bindings_generator_sources)',
        '<@(mojom_files)',
        '<(stamp_filename)',
      ],
      'outputs': [
        '<@(mojom_generated_outputs)',
      ],
      'action': [
        'python', '<@(mojom_bindings_generator)',
        '--use_bundled_pylibs', 'generate',
        '<@(mojom_files)',
        '-d', '<(DEPTH)',
        '<@(mojom_import_args)',
        '-o', '<(SHARED_INTERMEDIATE_DIR)',
        '--java_output_directory=<(java_out_dir)',
        '--variant', '<(mojom_variant)',
        '-g', '<(mojom_output_languages)',
        '<@(mojom_extra_generator_args)',
        '--bytecode_path',
        '<(SHARED_INTERMEDIATE_DIR)/mojo/public/tools/bindings',
      ],
      'message': 'Generating Mojo bindings from <@(mojom_files)',
    }
  ],
  'conditions': [
    ['require_interface_bindings==1', {
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/mojo/mojo_public.gyp:mojo_interface_bindings_generation',
        '<(DEPTH)/mojo/public/tools/bindings/bindings.gyp:precompile_mojom_bindings_generator_templates',
      ],
    }],
  ],
  # Prevent the generated sources from being injected into the "all" target by
  # preventing the code generator from being directly depended on by the "all"
  # target.
  'suppress_wildcard': '1',
  'hard_dependency': '1',
  'direct_dependent_settings': {
    # A target directly depending on this action will compile the generated
    # sources.
    'sources': [
      '<@(mojom_generated_outputs)',
    ],
    # Include paths needed to compile the generated sources into a library.
    'include_dirs': [
      '<(DEPTH)',
      '<(SHARED_INTERMEDIATE_DIR)',
    ],
    # Make sure the generated header files are available for any static library
    # that depends on a static library that depends on this generator.
    'hard_dependency': 1,
    'direct_dependent_settings': {
      # Include paths needed to find the generated header files and their
      # transitive dependancies when using the library.
      'include_dirs': [
        '<(DEPTH)',
        '<(SHARED_INTERMEDIATE_DIR)',
      ],
      'variables': {
        'generated_src_dirs': [
          '<(PRODUCT_DIR)/java_mojo/<(_target_name)/src',
        ],
        'additional_input_paths': [
          '<@(mojom_bindings_generator_sources)',
          '<@(mojom_files)',
        ],
        'mojom_generated_sources': [ '<@(mojom_generated_outputs)' ],
      },
    }
  },
}
