
###  NSLayoutConstraint

```
+ (instancetype)constraintWithItem:(id)view1
                         attribute:(NSLayoutAttribute)attr1
                         relatedBy:(NSLayoutRelation)relation
                            toItem:(nullable id)view2
                         attribute:(NSLayoutAttribute)attr2
                        multiplier:(CGFloat)multiplier
                          constant:(CGFloat)c;
```


### `MASViewAttribute`

```
@property (weak) UIView *view;
@property (weak) id item;
@property (assign) NSLayoutAttribute layoutAttribute;

```

构造方法,初始化变量`view `、`item `、`layoutAttribute `。

```
- (id)initWithView:(MAS_VIEW *)view item:(id)item layoutAttribute:(NSLayoutAttribute)layoutAttribute{
    _view = view;
    _item = item;
    _layoutAttribute = layoutAttribute;
}
```

判断layoutAttribute是否为`NSLayoutAttributeWidth`、`NSLayoutAttributeHeight`

```
- (BOOL)isSizeAttribute {
    return self.layoutAttribute == NSLayoutAttributeWidth
        || self.layoutAttribute == NSLayoutAttributeHeight;
}

```

重写isEqual方法<br>


*  1. 先判断是否是类MASViewAttribute的对象。
*  2.判断弱引用的view是否是同一个，layoutAttribute属性的值是否相等。
*  3. 重写hash的值

```
- (BOOL)isEqual:(MASViewAttribute *)viewAttribute {
    if ([viewAttribute isKindOfClass:self.class]) {
        return self.view == viewAttribute.view
            && self.layoutAttribute == viewAttribute.layoutAttribute;
    }
    return [super isEqual:viewAttribute];
}

- (NSUInteger)hash {
    return MAS_NSUINTROTATE([self.view hash], MAS_NSUINT_BIT / 2) ^ self.layoutAttribute;
}

```


###  `View+MASAdditions`

包含三个文件

```
View+MASAdditions.h
View+MASAdditions.m
View+MASShorthandAdditions.h

```

#####  `View+MASShorthandAdditions.h`

如果 `MAS_SHORTHAND`定义过，才能使用当前文件的功能

```
#ifdef MAS_SHORTHAND

#endif

```


```
MAS_ATTR_FORWARD(top);
```




###  关键字

*  NS_NOESCAPE : 用于修饰方法中的block类型参数。 作用是在方法返回之前就会执行完毕，而不是被保存起来在之后的某个时候再执行。