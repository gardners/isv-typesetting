#ifndef _isv_docx_h_
#define _isv_docx_h_

/* This information is needed for validating and interpreting the MS Office
 * Open XML for the ISV. */

/* note: there is special handling for w:smartTag, so is not included here */
static const char *keep_paths[] = {
    "w:document",
    "w:document/w:body",
    "w:document/w:body/w:p",                          /*       paragraph */
    "w:document/w:body/w:p/w:pPr",                    /* paragraph props */
    "w:document/w:body/w:p/w:pPr/w:pStyle",           /* paragraph style */
    "w:document/w:body/w:p/w:pPr/w:rPr/w:rStyle",     /*       run style */
    "w:document/w:body/w:p/w:r",                      /*             run */
    "w:document/w:body/w:p/w:r/w:rPr",                /*  run properties */
    "w:document/w:body/w:p/w:r/w:rPr/w:rStyle",       /*       run style */
    "w:document/w:body/w:p/w:r/w:rPr/w:smallCaps",    /*      small caps */
    "w:document/w:body/w:p/w:r/w:rPr/w:lang",         /*        language */
    "w:document/w:body/w:p/w:r/w:t",                  /*     text in run */
    "w:document/w:body/w:p/w:r/w:footnoteReference",  /* footnote in run */
    NULL
};

/* don't output warnings for these paths we may ignore */
static const char *other_valid_paths[] = {
    "w:document/w:body/w:sectPr/**",
    "w:document/w:body/**/w:bookmarkStart/**",
    "w:document/w:body/**/w:bookmarkEnd/**",
    "w:document/w:body/**/w:noProof",
    "w:document/w:body/w:p/w:pPr/w:spacing",
    "w:document/w:body/w:p/w:pPr/w:textAlignment",
    "w:document/w:body/w:p/w:pPr/w:keepNext",
    "w:document/w:body/w:p/w:pPr/w:keepLines",
    "w:document/w:body/w:p/w:pPr/w:outlineLvl",
    "w:document/w:body/w:p/w:pPr/w:ind",
    "w:document/w:body/w:p/w:pPr/w:tabs/**",
    "w:document/w:body/w:p/w:pPr/w:sectPr/**",
    "w:document/w:body/w:p/w:pPr/w:jc",
    "w:document/w:body/w:p/w:pPr/w:rPr",
    "w:document/w:body/w:p/w:pPr/w:rPr/w:rtl",
    "w:document/w:body/w:p/w:pPr/w:rPr/w:sz",
    "w:document/w:body/w:p/w:pPr/w:rPr/w:szCs",
    "w:document/w:body/w:p/w:pPr/w:rPr/w:rFonts",
    "w:document/w:body/w:p/w:proofErr",
    "w:document/w:body/w:p/w:r/w:lastRenderedPageBreak",
    "w:document/w:body/w:p/w:r/w:br",
    "w:document/w:body/w:p/w:r/w:tab",
    "w:document/w:body/w:p/w:r/w:fldChar",
    "w:document/w:body/w:p/w:r/w:instrText",
    "w:document/w:body/w:p/w:r/w:rPr/w:b",
    "w:document/w:body/w:p/w:r/w:rPr/w:i",
    "w:document/w:body/w:p/w:r/w:rPr/w:color",
    "w:document/w:body/w:p/w:r/w:rPr/w:rFonts",
    "w:document/w:body/w:p/w:r/w:rPr/w:w",
    "w:document/w:body/w:p/w:r/w:rPr/w:bCs",
    "w:document/w:body/w:p/w:r/w:rPr/w:iCs",
    "w:document/w:body/w:p/w:r/w:rPr/w:sz",
    "w:document/w:body/w:p/w:r/w:rPr/w:kern",
    "w:document/w:body/w:p/w:r/w:rPr/w:szCs",
    "w:document/w:body/w:p/w:r/w:rPr/w:vertAlign",
    "w:document/w:body/**/w:widowControl",
    NULL
};

#endif /* _isv_docx_h_ */
