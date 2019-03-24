/*
 * Copyright (c) 2019 Fabian Mastenbroek
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef BRAINIAC_PARSER_H
#define BRAINIAC_PARSER_H

#include <stdio.h>
#include <brainiac/ir.h>

/**
 * This module defines the library's parser interface for the brainfuck
 * language.
 *
 * @addtogroup parser Parser
 * @{
 */
/**
 * This structure represents the state of the parser.
 */
struct BrainiacParserContext {
    /**
     * The parser associated with this context.
     */
    const struct BrainiacParser *parser;

    /**
     * The program structure in which instructions are inserted.
     */
    struct BrainiacProgram *program;
};

/**
 * This structure represents a parser which parses a language into the
 * brainfuck-based intermediate representation defined in @ref ir.
 */
struct BrainiacParser {
    /**
     * A unique, full name of the parser implementation.
     */
    const char *name;

    /**
     * The version of the parser implementation.
     */
    const char *version;

    /**
     * An array of file extensions that this parser supports. The array is
     * terminated with a <code>NULL</code> pointer.
     */
    const char **extensions;

    /**
     * Allocate a {@link BrainiacParserContext}.
     *
     * @param[in] parser The parser to allocate a context for.
     * @param[in] program The program to insert the parsed instructions into.
     * May be <code>NULL</code>.
     * @return The allocated context or <code>NULL</code> on allocation failure.
     */
    struct BrainiacParserContext * (*alloc)(struct BrainiacParser *parser,
                                            struct BrainiacProgram *program);

    /**
     * Deallocate the specified {@link BrainiacParserContext}.
     *
     * @param[in] ctx The parser context to deallocate.
     */
    void (*dealloc)(struct BrainiacParserContext *ctx);

    /**
     * Indicate to the parser to start parsing. This might reset the parser's
     * internal state and clear the specified {@link BrainiacProgram}.
     *
     * @param[in] ctx The parser context to start.
     */
    void (*start)(struct BrainiacParserContext *ctx);

    /**
     * Consume the given string of characters.
     *
     * @param[in] ctx The parser context in which the characters should be
     * consumed.
     * @param[in] input The input string to consume.
     * @return {@link BRAINIAC_EOK} on success, one of the defined error codes
     * on failure.
     */
    int (*consume)(struct BrainiacParserContext *ctx, const char *input);

    /**
     * Indicate to the parser that the end of the input has been reached.
     *
     * @param[ctx] ctx The parser context to indicate to.
     * @return {@link BRAINIAC_EOK} on success, one of the defined error codes
     * on failure.
     */
    int (*end)(struct BrainiacParserContext *ctx);
};

/**
 * Allocate a {@link BrainiacParserContext}.
 *
 * @param[in] parser The parser to allocate a context for.
 * @param[in] program The program to insert the parsed instructions into.
 * May be <code>NULL</code>.
 * @return The allocated context or <code>NULL</code> on allocation failure.
 */
struct BrainiacParserContext * brainiac_parser_alloc(struct BrainiacParser *parser,
                                                     struct BrainiacProgram *program);

/**
 * Deallocate the specified {@link BrainiacParserContext}.
 *
 * @param[in] ctx The parser context to deallocate.
 */
void brainiac_parser_dealloc(struct BrainiacParserContext *ctx);

/**
 * Indicate to the parser to start parsing. This might reset the parser's
 * internal state and clear the specified {@link BrainiacProgram}.
 *
 * @param[in] ctx The parser context to start.
 */
void brainiac_parser_start(struct BrainiacParserContext *ctx);

/**
 * Parse the given string.
 *
 * @param[in] ctx The parser context in which to consume the string.
 * @param[in] input The string to parse.
 * @return {@link BRAINIAC_EOK} on success, one of the defined error codes
 * on failure.
 */
int brainiac_parser_consume_string(struct BrainiacParserContext *ctx,
                                   const char *input);

/**
 * Parse the given file.
 *
 * @param[in] ctx The parser context in which to consume the file.
 * @param[in] file The handle to the file to parse.
 * @return {@link BRAINIAC_EOK} on success, one of the defined error codes
 * on failure.
 */
int brainiac_parser_consume_file(struct BrainiacParserContext *ctx,
                                 FILE *file);

/**
 * Indicate to the parser that the end of the input has been reached.
 *
 * @param[in] ctx The parser context to indicate to.
 * @return {@link BRAINIAC_EOK} on success, one of the defined error codes
 * on failure.
 */
int brainiac_parser_end(struct BrainiacParserContext *ctx);

/**
 * Retrieve an array of available parsers on this platform.
 *
 * @return A <code>NULL</code>-terminated array of available parsers.
 */
struct BrainiacParser ** brainiac_parser_list(void);

/**
 * Find the first parser available with the given name.
 *
 * @param[in] name The name of the parser to find.  If <code>NULL</code>
 * is given, then the first vm that is found will be returned.
 * @return The found parser or <code>NULL</code> if it could not be
 * found.
 */
struct BrainiacParser * brainiac_parser_find(const char *name);

/**
 * Find the first parser that supports the specified file extension.
 *
 * @param[in] ext The file extension to find a parser for.
 * @return The found parser or <code>NULL</code> if it could not be
 * found.
 */
struct BrainiacParser * brainiac_parser_find_by_extension(const char *ext);
/** @} */

#endif /* BRAINIAC_PARSER_H */