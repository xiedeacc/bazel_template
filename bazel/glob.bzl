def optional_glob(include, exclude = [], allow_empty = True, **kwargs):
    return native.glob(
        include,
        exclude = exclude,
        allow_empty = allow_empty,
        **kwargs
    )
