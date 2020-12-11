
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


### MASConstraint

子类：`MASViewConstraint`和`MASCompositeConstraint`


MASConstraint 代码块功能可分为三种。

* 创建新的MASConstraint类对象

```
- (MASConstraint *)left;
- (MASConstraint *)top;
- (MASConstraint *)right;
- (MASConstraint *)bottom;
- (MASConstraint *)leading;
- (MASConstraint *)trailing;
```
```
- (MASConstraint *)right {
    return [self addConstraintWithLayoutAttribute:NSLayoutAttributeRight];
}
```
`MASViewConstraint`中的`addConstraintWithLayoutAttribute`方法会执行`MASConstraintMaker`中的方法。【按照代码的执行顺序粘贴代码】

```
- (MASConstraint *)addConstraintWithLayoutAttribute:(NSLayoutAttribute)layoutAttribute {
    return [self.delegate constraint:self addConstraintWithLayoutAttribute:layoutAttribute];
}

```

执行`make.left.right`就会将`make.left`生成的`constraint ` 对象替换为复合型`compositeConstraint `对象。【这里要记得这种转变，下一步要使用】

```
- (MASConstraint *)constraint:(MASConstraint *)constraint c:(NSLayoutAttribute)layoutAttribute {
    MASViewAttribute *viewAttribute = [[MASViewAttribute alloc] initWithView:self.view layoutAttribute:layoutAttribute];
    MASViewConstraint *newConstraint = [[MASViewConstraint alloc] initWithFirstViewAttribute:viewAttribute];
    if ([constraint isKindOfClass:MASViewConstraint.class]) {
        //replace with composite constraint
        NSArray *children = @[constraint, newConstraint];
        MASCompositeConstraint *compositeConstraint = [[MASCompositeConstraint alloc] initWithChildren:children];
        compositeConstraint.delegate = self;
        [self constraint:constraint shouldBeReplacedWithConstraint:compositeConstraint];
        return compositeConstraint;
    }
}
```

接着执行`make.left.right.equal`语句，`equalTo `会调用`MASCompositeConstraint`文件中的方法。

```
- (MASConstraint * (^)(id))equalTo {
    return ^id(id attribute) {
        return self.equalToWithRelation(attribute, NSLayoutRelationEqual);
    };
}
```

调用`MASCompositeConstraint`的`equalToWithRelation `方法

```
- (MASConstraint * (^)(id, NSLayoutRelation))equalToWithRelation {
    return ^id(attribute, NSLayoutRelationEqual) {
    // 遍历数组调用MASConstraint的equalToWithRelation方法
        for (MASConstraint *constraint in self.childConstraints.copy) {
            constraint.equalToWithRelation(attr, relation);
        }
        return self;
    };
}

```

调用`MASConstraint`的`equalToWithRelation `方法。设置`layoutRelation`和`secondViewAttribute`。到此构建`NSLayoutConstraint `对象所需的元素都已经有了。

```
- (MASConstraint * (^)(id, NSLayoutRelation))equalToWithRelation {
    return ^id(id attribute, NSLayoutRelation relation) {
            NSAssert(!self.hasLayoutRelation || self.layoutRelation == relation && [attribute isKindOfClass:NSValue.class], @"Redefinition of constraint relation");
            self.layoutRelation = relation;
            self.secondViewAttribute = attribute;//这里会区分不同的类型
            return self;
    };
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

如果 `MAS_SHORTHAND`定义过，才能使用当前文件的功能。

将`MAS_ATTR_FORWARD `展开，发现实际上是`[self mas_##attr]`,比如`top `实际上执行的是`mas_top `，将会跳转到`View+MASAdditions`执行对应的方法。(这里需要注意`##`,在预处理时的作用)

```
MAS_ATTR_FORWARD(top);
```

```
- (MASViewAttribute *)top {    
   return [self mas_top];  
}
```

以下方法像上述一样，会跳转到`View+MASAdditions`执行对应的方法。

```
- (NSArray *)makeConstraints:(void(^)(MASConstraintMaker *make))block;
- (NSArray *)updateConstraints:(void(^)(MASConstraintMaker *make))block;
- (NSArray *)remakeConstraints:(void(^)(MASConstraintMaker *make))block;
```


总结：`View+MASShorthandAdditions`中的方法将`mas_top`改写成`top`这种写法，个人感觉也没提升多少效率。

#####  `View+MASAdditions`扩展UIView方法

```
- (NSArray *)mas_makeConstraints:(void(NS_NOESCAPE ^)(MASConstraintMaker *make))block;
- (NSArray *)mas_updateConstraints:(void(NS_NOESCAPE ^)(MASConstraintMaker *make))block;
- (NSArray *)mas_remakeConstraints:(void(NS_NOESCAPE ^)(MASConstraintMaker *make))block;

```

```
    self.translatesAutoresizingMaskIntoConstraints = NO;
    //创建一个MASConstraintMaker对象 constraintMaker
    MASConstraintMaker *constraintMaker = [[MASConstraintMaker alloc] initWithView:self];
    //通过配置left,top,size属性生成MASConstraint的对象，并放在constraintMaker中的数组list中，这样的写法“make.left”
      // 
    block(constraintMaker);
    return [constraintMaker install];
```

###  关键字

*  NS_NOESCAPE : 用于修饰方法中的block类型参数。 作用是在方法返回之前就会执行完毕，而不是被保存起来在之后的某个时候再执行。
*  `##`在预处理阶段的作用，强制连接。