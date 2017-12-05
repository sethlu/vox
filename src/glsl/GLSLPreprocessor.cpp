
#include <iostream>
#include "GLSLPreprocessor.h"


namespace renderbox {

    void GLSLPreprocessor::lex(GLSLToken &token) {

        bool returnedToken;
        do {
            returnedToken = currentLexer->lex(token);
        } while (!returnedToken);

    }

    void GLSLPreprocessor::enterMainSource(const char *filename) {
        enterSource(filename);
    }

    void GLSLPreprocessor::enterSource(const char *filename) {

        GLSLPreprocessorSource *source;
        auto it = sources.find(filename);
        if (it == sources.end()) {
            source = new GLSLPreprocessorSource(filename);
            sources.insert(std::pair<const char *, std::unique_ptr<GLSLPreprocessorSource>>(
                filename, std::unique_ptr<GLSLPreprocessorSource>(source)));
        } else {
            source = it->second.get();
        }

        const char *bufferStart = source->source.get();
        const char *bufferEnd = bufferStart + source->size;

        currentLexer = new GLSLPreprocessorLexer(this, bufferStart, bufferStart, bufferEnd);
        lexers.push_back(std::unique_ptr<GLSLPreprocessorLexer>(currentLexer));

    }

    void GLSLPreprocessor::exitSource() {

        lexers.pop_back();
        currentLexer = lexers.back().get();

        // TODO: Count source file reference and dealloc its memory

    }

    GLSLPPKeywordKind getPPKeywordKind(const char *keyword, unsigned len) {

#define HASH(LEN, FIRST, THIRD) \
  (((LEN) << 5) + ((((FIRST) - 'a') + ((THIRD) - 'a')) & 31))
#define CASE(LEN, FIRST, THIRD, NAME) \
    case HASH(LEN, FIRST, THIRD): \
        return memcmp(keyword, #NAME, LEN) ? pp_not_keyword : pp_ ## NAME

        switch (HASH(len, keyword[0], keyword[2])) {
            CASE(7, 'i', 'c', include);
            default: return pp_not_keyword;
        }

#undef CASE
#undef HASH

    }

    void GLSLPreprocessor::handleDirective(GLSLToken &token) {

        GLSLToken hash = token;

        // Get the keyword
        lex(token);

        if (token.kind == eod || token.len < 2) { // Empty line or not a directive

            NotKeyword:

            // Skip to the end of line
            currentLexer->isPreprocessingDirective = false;
            lex(token);

            token.kind = unknown;
            token.len = static_cast<unsigned>(token.pointer + token.len - hash.pointer);
            token.pointer = hash.pointer;

            return;

        }

        // Make cleaned keyword
        char keyword[token.len + 1];
        memcpy(keyword, token.pointer, token.len);
        keyword[token.len] = '\0';

        switch (getPPKeywordKind(keyword, token.len)) {
            default:
                break;

            case pp_include: // Include directive
                break;
        }

        goto NotKeyword;

    }

    GLSLPreprocessorSource::GLSLPreprocessorSource(const char *filename) {
        FILE *f = fopen(filename, "r");

        // Determine file size
        fseek(f, 0, SEEK_END);
        long size_ = ftell(f);

        if (size_ < 0) throw 2; // Failed to tell size
        size = static_cast<size_t>(size_);

        source.reset(new char[size]);

        rewind(f);
        fread(source.get(), sizeof(char), size, f);

    }

    GLSLPreprocessorLexer::GLSLPreprocessorLexer(GLSLPreprocessor *preprocessor,
                                                 const char *bufferStart,
                                                 const char *bufferPointer,
                                                 const char *bufferEnd)
        : preprocessor(preprocessor), bufferStart(bufferStart), bufferPointer(bufferPointer), bufferEnd(bufferEnd) {
        isAtPhysicalStartOfLine = true;
        isPreprocessingDirective = false;
    }

    bool GLSLPreprocessorLexer::lex(GLSLToken &token) {

        LexNextToken:

        skipHorizontalWhitespace(token);

        // Get and advance pointer
        const char *pointer = bufferPointer;

        switch (*pointer++) {
            default: {
                // A normal line of code

                LexLine:

                return lexLine(token, pointer);

            }

            case '\0': // End of file

                if (isPreprocessingDirective) {
                    // Append eod before the end of file

                    token.kind = eod;
                    token.pointer = bufferPointer;
                    token.len = 1;

                    isPreprocessingDirective = false;

                    break;
                }

                token.kind = eof;
                token.pointer = bufferPointer;
                token.len = 1;

                bufferPointer = pointer;

                break;

            case '\n':

                isAtPhysicalStartOfLine = true;

                if (isPreprocessingDirective) {
                    token.kind = eod;
                    token.pointer = bufferPointer;
                    token.len = 1;

                    bufferPointer = pointer;
                    isPreprocessingDirective = false;

                    break;
                }

                bufferPointer = pointer;

                goto LexNextToken;

            case '#': // Directive

                if (isAtPhysicalStartOfLine) {

                    // Hash
                    token.kind = unknown;
                    token.pointer = bufferPointer;
                    token.len = 1;

                    bufferPointer = pointer;

                    isPreprocessingDirective = true;

                    preprocessor->handleDirective(token);

                    break;

                }

                goto LexLine;

            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
            case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
            case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
            case 'V': case 'W': case 'X': case 'Y': case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
            case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
            case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
            case 'v': case 'w': case 'x': case 'y': case 'z':
            case '_':

                // Only lex identifier if in directive
                if (!isPreprocessingDirective) goto LexLine;
                return lexIdentifier(token, pointer);

        }

        return true;

    }

    bool GLSLPreprocessorLexer::lexLine(GLSLToken &token, const char *pointer) {

        while (*pointer != '\0' && *pointer != '\n') ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::lexIdentifier(GLSLToken &token, const char *pointer) {

        while (isIdentifierBody(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = identifier;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::skipHorizontalWhitespace(GLSLToken &token) {

        const char *pointer = bufferPointer;

        while (isHorizontalWhitespace(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

}